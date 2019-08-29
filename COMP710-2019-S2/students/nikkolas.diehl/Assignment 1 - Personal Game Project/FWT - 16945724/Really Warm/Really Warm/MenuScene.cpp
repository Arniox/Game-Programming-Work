#include "MenuScene.h"



MenuScene::MenuScene()
{
}
MenuScene::~MenuScene()
{
}

void 
MenuScene::Initialise(IniParser * m_iniParser, BackBuffer * m_pBackBuffer, int & screenWidth, int & screenHeight)
{
}

void 
MenuScene::Draw(BackBuffer & backBuffer)
{
	//Draw Text for title
	backBuffer.SetTextColour(255, 255, 255);
	string titleOutPut = "Space Invaders!";
	backBuffer.DrawText(20, 50, titleOutPut.c_str(), 0);
}

void 
MenuScene::Process(float deltaTime)
{
}
