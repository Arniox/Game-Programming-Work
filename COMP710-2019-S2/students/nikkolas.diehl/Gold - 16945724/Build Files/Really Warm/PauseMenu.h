#ifndef __PAUSEMENU_H__
#define __PAUSEMENU_H__

//Locals
#include "MenuFrame.h"
#include "backbuffer.h"
#include "iniParser.h"
#include "fmod.h"
#include "inputhandler.h"
#include "UIElement.h"

//Libraries
#include <vector>

class PauseMenu:
	public MenuFrame
{
public:
	PauseMenu();
	~PauseMenu();

	void Initialise(IniParser* iniParser, BackBuffer* backBuffer, int screenWidth, int screenHeight);
	void Process(InputHandler* inputHandler, double deltaTime);
	void Draw(BackBuffer& backBuffer);

	bool RequestBack();
	bool RequestContinue();

	void SetLoose(bool loose);

protected:
	//Functions
	void SetTitleToken(std::string _newToken);
	
	enum Button {
		CONTINUE = 0,
		BACK = 1,
		EXIT = 2
	};

	UIElement* titleCard;
	bool looseScreen;
	
	bool mo_b_backButton;
	bool mo_b_continueButton;

};

#endif

