#pragma once

#include "iniParser.h"
#include "backbuffer.h"

class MenuScene
{
public:
	MenuScene();
	~MenuScene();

	//Main functionality
	void Initialise(IniParser* m_iniParser, BackBuffer* m_pBackBuffer, int& screenWidth, int& screenHeight);
	void Draw(BackBuffer& backBuffer);
	void Process(float deltaTime);


};

