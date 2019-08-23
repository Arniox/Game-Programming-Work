#pragma once
#include "iniParser.h"
#include "backbuffer.h"

class MenuScene
{
public:
	MenuScene();
	~MenuScene();

	//Main functionality
	void MenuScene::Initialise(IniParser* m_iniParser, BackBuffer* m_pBackBuffer, int& screenWidth, int& screenHeight);
	void MenuScene::Draw(BackBuffer& backBuffer);
	void MenuScene::Process(float deltaTime);


};

