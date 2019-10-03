#include "PauseMenu.h"
#include "SceneManager.h"


PauseMenu::PauseMenu()
: mo_b_backButton(false)
, mo_b_continueButton(false)
, titleCard()
, looseScreen(false)
{
	titleCard = new UIElement();

	//Buttons
	uiElementArray.push_back(new UIElement());
	uiElementArray.push_back(new UIElement());
	uiElementArray.push_back(new UIElement());
}
PauseMenu::~PauseMenu()
{
	delete titleCard;
	titleCard = nullptr;

	for (std::vector<UIElement*>::iterator it = uiElementArray.begin(); it != uiElementArray.end(); ++it) {
		delete (*it);
		(*it) = nullptr;
	}

	uiElementArray.clear();
	uiElementArray.shrink_to_fit();
}

void 
PauseMenu::Initialise(IniParser* iniParser, BackBuffer* backBuffer, int screenWidth, int screenHeight)
{
	//Buttons
	uiElementArray[CONTINUE]->CreateAnimatedUIElement("assets/Images/buttonSpriteSheet.png", backBuffer, "CONTINUE", screenWidth / 2, static_cast<int>(screenHeight * 0.30),
		BUTTON_SCALE, FRAME_COUNT, PANEL_COUNT, FRAME_SPEED, BTN_FONT);
	uiElementArray[BACK]->CreateAnimatedUIElement("assets/Images/buttonSpriteSheet.png", backBuffer, "BACK", screenWidth / 2, static_cast<int>(screenHeight * 0.50),
		BUTTON_SCALE, FRAME_COUNT, PANEL_COUNT, FRAME_SPEED, BTN_FONT);
	uiElementArray[EXIT]->CreateAnimatedUIElement("assets/Images/buttonSpriteSheet.png", backBuffer, "QUIT", screenWidth / 2, static_cast<int>(screenHeight * 0.90),
		0.5, FRAME_COUNT, PANEL_COUNT, FRAME_SPEED, BTN_FONT);

	//Title card
	int fontType = iniParser->GetValueAsInt("PauseMenu", "fontType");
	std::string spriteImage = iniParser->GetValueAsString("PauseMenu", "sprite");

	titleCard->CreateUIElement(spriteImage, backBuffer, "PAUSED", screenWidth / 2, 0,
		1, fontType);
}

void 
PauseMenu::Process(InputHandler* inputHandler, double deltaTime)
{
	previousSelected = selected;

	//Change selected also based on mouse
	//The selected state being set based on the mouse should theoretically never ever
	//Go beyond the set number of buttons so no need to check for over limit
	
	if (uiElementArray[CONTINUE]->GetBox()->Contains(&*inputHandler->GetMousePos())) {
		if (!looseScreen) {
			selected = CONTINUE;
		}
	}
	else if (uiElementArray[BACK]->GetBox()->Contains(&*inputHandler->GetMousePos())) {
		selected = BACK;
	}
	else if (uiElementArray[EXIT]->GetBox()->Contains(&*inputHandler->GetMousePos())) {
		selected = EXIT;
	}

	//Delay
	if (delayCount > 0) {
		delayCount--;
	}
	else {
		//Increment selected with keyboard buttons and later controller buttons
		if (inputHandler->GetKeyBoardLayout("up") || inputHandler->GetKeyBoardLayout("w")) {
			selected--;
			//Reset selected if you've gone too small
			if (selected < (!looseScreen ? 0 : 1)) {
				selected = static_cast<signed int>(uiElementArray.size()) - 1;
			}
			//Delay till the next button press
			delayCount = MAX_DELAY;
		}
		if (inputHandler->GetKeyBoardLayout("down") || inputHandler->GetKeyBoardLayout("S")) {
			selected++;
			//Reset selected if you've gone too large
			if (selected > static_cast<signed int>(uiElementArray.size()) - 1) {
				selected = (!looseScreen ? 0 : 1);
			}
			//Delay till the next button press
			delayCount = MAX_DELAY;
		}
	}

	//Set selected to hover state and if clicked enter, mouse button or controller button
	//Then activate given button
	bool activate = inputHandler->GetKeyBoardLayout("enter") || inputHandler->GetMouseClickStates()[0];
	//Reset hover states
	mo_b_continueButton = false;
	mo_b_backButton = false;
	ResetButtonHoverStates(uiElementArray);
	//Update hover states
	switch (selected) {
		case CONTINUE:
			//Set hover state
			uiElementArray[CONTINUE]->GetAnimatedSprite()->SetCurrentPanel(CLICKED);
			if (activate) {
				mo_b_continueButton = (!looseScreen ? true : false);
			}
			break;
		case BACK:
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
PauseMenu::Draw(BackBuffer& backBuffer)
{
	titleCard->DrawUIElement(backBuffer, 110, true);

	int index = 0;
	for (std::vector<UIElement*>::iterator it = uiElementArray.begin(); it != uiElementArray.end(); ++it) {
		if (looseScreen && index == CONTINUE) {
			index++;
			continue;
		}
		(*it)->DrawUIElement(backBuffer, 110, false);

		index++;
	}

}

bool
PauseMenu::RequestContinue()
{
	return mo_b_continueButton;
}

void
PauseMenu::SetLoose(bool loose)
{
	if (loose) {
		SetTitleToken("YOU LOOSE!");
		looseScreen = true;
	}
}

void 
PauseMenu::SetTitleToken(std::string _newToken)
{
	titleCard->UpdateElementText(_newToken);
	uiElementArray[BACK]->UpdateElementText("Restart!");
}

bool
PauseMenu::RequestBack()
{
	return mo_b_backButton;
}
