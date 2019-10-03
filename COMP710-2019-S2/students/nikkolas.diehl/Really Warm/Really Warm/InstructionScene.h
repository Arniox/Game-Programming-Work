#ifndef __INSTRUCTIONSCENE_H__
#define __INSTRUCTIONSCENE_H__

//Locals
#include "MenuFrame.h"
#include "backbuffer.h"
#include "iniParser.h"
#include "fmod.h"
#include "inputhandler.h"
#include "UIElement.h"

//Libraries
#include <vector>

class InstructionScene:
	public MenuFrame
{
public:
	InstructionScene();
	~InstructionScene();

	void Initialise(IniParser* iniParser, BackBuffer* backBuffer, int screenWidth, int screenHeight);
	void Process(InputHandler* inputHandler, double deltaTime);
	void Draw(BackBuffer& backBuffer);

	bool RequestBack();

protected:
	enum Button {
		BACK = 0,
		EXIT = 1
	};

	std::vector<UIElement*> instructions;

	bool mo_b_backButton;
};

#endif

