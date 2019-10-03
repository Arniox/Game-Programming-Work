#include "MainMenuScene.h"
#include "SceneManager.h"

MainMenuScene::MainMenuScene()
: mo_b_playButton(false)
, mo_b_instructionButton(false)
{
	uiElementArray.push_back(new UIElement());
	uiElementArray.push_back(new UIElement());
	uiElementArray.push_back(new UIElement());
}
MainMenuScene::~MainMenuScene()
{
	for (std::vector<UIElement*>::iterator it = uiElementArray.begin(); it != uiElementArray.end(); ++it) {
		delete (*it);
		(*it) = nullptr;
	}

	uiElementArray.clear();
	uiElementArray.shrink_to_fit();
}

void 
MainMenuScene::Initialise(BackBuffer* backBuffer, int screenWidth, int screenHeight)
{
	//splashImg->CreateUIElement("assets/Images/unclickedButton.png", backBuffer, "", 0, 0, 1, 1);
	uiElementArray[PLAY]->CreateAnimatedUIElement("assets/Images/buttonSpriteSheet.png", backBuffer, "PLAY", screenWidth / 2, static_cast<int>(screenHeight * 0.10),
		BUTTON_SCALE, FRAME_COUNT, PANEL_COUNT, FRAME_SPEED, BTN_FONT);
	uiElementArray[INSTRUCT]->CreateAnimatedUIElement("assets/Images/buttonSpriteSheet.png", backBuffer, "INSTRUCTIONS", screenWidth / 2, static_cast<int>(screenHeight * 0.30),
		BUTTON_SCALE, FRAME_COUNT, PANEL_COUNT, FRAME_SPEED, BTN_FONT);
	uiElementArray[EXIT]->CreateAnimatedUIElement("assets/Images/buttonSpriteSheet.png", backBuffer, "QUIT", screenWidth / 2, static_cast<int>(screenHeight * 0.90),
		0.5, FRAME_COUNT, PANEL_COUNT, FRAME_SPEED, BTN_FONT);
}

void 
MainMenuScene::Process(InputHandler* inputHandler, double deltaTime)
{
	previousSelected = selected;

	//Change selected also based on mouse
	//The selected state being set based on the mouse should theoretically never ever
	//Go beyond the set number of buttons so no need to check for over limit
	if (uiElementArray[PLAY]->GetBox()->Contains(&*inputHandler->GetMousePos())) {
		selected = PLAY;
	}
	else if (uiElementArray[INSTRUCT]->GetBox()->Contains(&*inputHandler->GetMousePos())) {
		selected = INSTRUCT;
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

	//Set selected to hover state and if clicked enter, mouse button or controller button
	//Then activate given button
	bool activate = inputHandler->GetKeyBoardLayout("enter") || inputHandler->GetMouseClickStates()[0];
	//Reset hover states
	mo_b_playButton = false;
	mo_b_instructionButton = false;
	ResetButtonHoverStates(uiElementArray);
	//Update hover states
	switch (selected) {
		case PLAY:
			//Set hover state
			uiElementArray[PLAY]->GetAnimatedSprite()->SetCurrentPanel(CLICKED);
			if (activate) {
				mo_b_playButton = true;
			}
			break;
		case INSTRUCT:
			//Set hover state
			uiElementArray[INSTRUCT]->GetAnimatedSprite()->SetCurrentPanel(CLICKED);
			if (activate) {
				mo_b_instructionButton = true;
			}
			break;
		case EXIT:
			//Set hover state
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
MainMenuScene::Draw(BackBuffer & backBuffer)
{
	for (std::vector<UIElement*>::iterator it = uiElementArray.begin(); it != uiElementArray.end(); ++it) {
		(*it)->DrawUIElement(backBuffer, 255, false);
	}
}

bool 
MainMenuScene::RequestPlay()
{
	return mo_b_playButton;
}

bool 
MainMenuScene::RequestInstruction()
{
	return mo_b_instructionButton;
}
