#include "InstructionScene.h"
#include "SceneManager.h"

InstructionScene::InstructionScene()
: mo_b_backButton(false)
, instructions()
{
	//Buttons
	uiElementArray.push_back(new UIElement());
	uiElementArray.push_back(new UIElement());

	//Instruction lists
	instructions.push_back(new UIElement()); //Part 1
}
InstructionScene::~InstructionScene()
{
	for (std::vector<UIElement*>::iterator it = instructions.begin(); it != instructions.end(); ++it) {
		delete (*it);
		(*it) = nullptr;
	}

	for (std::vector<UIElement*>::iterator it = uiElementArray.begin(); it != uiElementArray.end(); ++it) {
		delete (*it);
		(*it) = nullptr;
	}
	uiElementArray.clear();
	uiElementArray.shrink_to_fit();

	instructions.clear();
	instructions.shrink_to_fit();
}

void 
InstructionScene::Initialise(IniParser* iniParser, BackBuffer* backBuffer, int screenWidth, int screenHeight)
{
	uiElementArray[BACK]->CreateAnimatedUIElement("assets/Images/buttonSpriteSheet.png", backBuffer, "BACK", screenWidth / 2, static_cast<int>(screenHeight * 0.60), 
		BUTTON_SCALE, FRAME_COUNT, PANEL_COUNT, FRAME_SPEED, BTN_FONT);
	uiElementArray[EXIT]->CreateAnimatedUIElement("assets/Images/buttonSpriteSheet.png", backBuffer, "QUIT", screenWidth / 2, static_cast<int>(screenHeight * 0.90),
		0.5, FRAME_COUNT, PANEL_COUNT, FRAME_SPEED, BTN_FONT);

	//Insturction lists
	int instructionFont = iniParser->GetValueAsInt("Instructions", "fontType");
	std::string spriteImage = iniParser->GetValueAsString("Instructions", "sprite");

	instructions[0]->CreateUIElement(spriteImage, backBuffer,
		iniParser->GetValueAsString("Instructions", "part1") + iniParser->GetValueAsString("Instructions", "part2") + iniParser->GetValueAsString("Instructions", "part3"),
		screenWidth / 2, static_cast<int>(screenHeight * 0.08), 1, instructionFont);
}

void 
InstructionScene::Process(InputHandler* inputHandler, double deltaTime)
{
	previousSelected = selected;

	//Change selected also based on mouse
	//The selected state being set based on the mouse should theoretically never ever
	//Go beyond the set number of buttons so no need to check for over limit

	if (uiElementArray[BACK]->GetBox()->Contains(&*inputHandler->GetMousePos())) {
		selected = BACK;
	}
	else if (uiElementArray[EXIT]->GetBox()->Contains(&*inputHandler->GetMousePos())) {
		selected = EXIT;
	}

	if (delayCount > 0) {
		delayCount--;
	}
	else {
		//Increment selected with keyboard buttons and later controller buttons
		if (inputHandler->GetKeyBoardLayout("up") || inputHandler->GetKeyBoardLayout("w")) {
			selected--;
			//Reset selected if you've gone too small
			if (selected < 0) {
				selected = static_cast<signed int>(uiElementArray.size()) - 1;
			}
			//Delay till the next button press
			delayCount = MAX_DELAY;
		}
		if (inputHandler->GetKeyBoardLayout("down") || inputHandler->GetKeyBoardLayout("s")) {
			selected++;
			//Reset selected if you've gone too large
			if (selected > static_cast<signed int>(uiElementArray.size()) - 1) {
				selected = 0;
			}
			//Delay till the next button press
			delayCount = MAX_DELAY;
		}
	}

	//Set selected to hover state and if clicked entier, mouse button or controller button
	//Then activate given button
	bool activate = inputHandler->GetKeyBoardLayout("enter") || inputHandler->GetMouseClickStates()[0];
	//Reset hover states
	mo_b_backButton = false;
	ResetButtonHoverStates(uiElementArray);
	//Update hover states
	switch (selected) {
		case BACK:
			//Set hover state
			uiElementArray[BACK]->GetAnimatedSprite()->SetCurrentPanel(CLICKED);
			if (activate) {
				mo_b_backButton = true;
			}
			break;
		case EXIT:
			uiElementArray[EXIT]->GetAnimatedSprite()->SetCurrentPanel(CLICKED);
			if (activate) {
				inputHandler->ExitGame();
			}
			break;
		default:
			//No button selected
			break;
	}

	//If selected is now different to previous selected, then request to play a sound
	if (previousSelected != selected) {
		BUTTON_HOVER_BOOL = true;
	}
	else {
		BUTTON_HOVER_BOOL = false;
	}
}

void 
InstructionScene::Draw(BackBuffer& backBuffer)
{
	for (std::vector<UIElement*>::iterator it = uiElementArray.begin(); it != uiElementArray.end(); ++it) {
		(*it)->DrawUIElement(backBuffer, 255, false);
	}

	for (std::vector<UIElement*>::iterator itTwo = instructions.begin(); itTwo != instructions.end(); ++itTwo) {
		(*itTwo)->DrawUIElement(backBuffer, 255, true);
	}
}

bool 
InstructionScene::RequestBack()
{
	return mo_b_backButton;
}
