#include "InGameScene.h"
#include "inputhandler.h"
#include <time.h>


InGameScene::InGameScene()
:mx_player(0)
{
}
InGameScene::~InGameScene()
{
	//Empty sound
	UnloadFMOD();
}

void 
InGameScene::Initialise(IniParser* m_iniParser, BackBuffer* m_pBackBuffer, int& screenWidth, int& screenHeight)
{
	//Init Fmod
	InitFMOD();

	//--------------Member data---------------------
	m_windowWidth = screenWidth;
	m_windowHeight = screenHeight;

	//--------------Ingame entities-----------------
	mx_player = new Player();
	mx_player->Initialise(m_pBackBuffer);
	mx_player->SetCenter((float)((screenWidth/2)-10), (float)(0));

	//--------------Walls---------------------------
	wallsTest = new Walls();
	wallsTest->CreateWall(m_pBackBuffer, 20.0f, 400.0f,    //x1 y1
										 1000.0f, 400.0f); //x2 y2
}

void
InGameScene::Process(InputHandler* ep_inputHander, float deltaTime)
{
	//Process sound
	m_fmodSystem->update();

	//Process entities
	//Player
	bool test = ep_inputHander->GetKeyBoardLayout("d");
	mx_player->Process(ep_inputHander, wallsTest, deltaTime);

}

void 
InGameScene::Draw(BackBuffer& backBuffer, int& m_FPS, int& m_totalFramesPassed)
{
	backBuffer.SetClearColour(0, 0, 0);

	//Draw entities
	//Player
	mx_player->Draw(backBuffer);
	wallsTest->Draw(backBuffer);
	
}

void
InGameScene::InitFMOD()
{
	FMOD::System_Create(&m_fmodSystem);
	m_fmodSystem->init(32, FMOD_INIT_NORMAL, 0);
}

void
InGameScene::UnloadFMOD()
{

}
