#ifndef __INSTRUCTIONMENU_H__
#define __INSTRUCTIONMENU_H__

//Locals
#include "State.h"

//Libraries
#include <vector>

//Forward Declerations
class BackBuffer;
class GUI;

class InstructionMenu:
	public State
{
public:
	InstructionMenu();
	~InstructionMenu();

	void Proccess(float deltaTime);
	void Draw(BackBuffer & backBuffer);
	void ProcessControls();
protected:
	GUI* m_pGUI;
};

#endif

