#ifndef __MAINMENUSCENE_H__
#define __MAINMENUSCENE_H__

//Locals
#include "MenuFrame.h"
#include "backbuffer.h"
#include "iniParser.h"
#include "fmod.h"
#include "inputhandler.h"
#include "UIElement.h"

//Libraries
#include <time.h>
#include <vector>

class MainMenuScene:
	public MenuFrame
{
public:
	MainMenuScene();
	~MainMenuScene();

	void Initialise(BackBuffer* backBuffer, int screenWidth, int screenHeight);
	void Process(InputHandler* inputHandler, double deltaTime);
	void Draw(BackBuffer& backBuffer);

	bool RequestPlay();
	bool RequestInstruction();

protected:
	enum Button {
		PLAY = 0,
		INSTRUCT = 1,
		EXIT = 2
	};

	bool mo_b_playButton;
	bool mo_b_instructionButton;
};

#endif

