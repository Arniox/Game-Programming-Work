#ifndef __MAINMENU_H__
#define __MAINMENU_H__

//Locals
#include "State.h"

//Libraries
#include <vector>

//Forward declerations
class BackBuffer;
class GUI;

class MainMenu:
	public State
{
public:
	MainMenu();
	~MainMenu();

	void Proccess(float deltaTime);
	void Draw(BackBuffer & backBuffer);
	void ProcessControls();
protected:
	GUI* m_pGUI;
};

#endif

