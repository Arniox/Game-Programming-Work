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

	delete m_wallController;
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
	mx_player->SetCenter(static_cast<float>((screenWidth/2)-10), static_cast<float>(0));

	//--------------Walls---------------------------
	m_wallController = new WallController();
	m_wallController->Initialise(m_pBackBuffer, screenWidth, screenHeight);
}

void
InGameScene::Process(InputHandler* ep_inputHander, float deltaTime)
{
	//Process sound
	m_fmodSystem->update();

	//Process entities
	//Player
	bool test = ep_inputHander->GetKeyBoardLayout("d");
	mx_player->Process(ep_inputHander, m_wallController, deltaTime);

}

void 
InGameScene::Draw(BackBuffer& backBuffer, int& m_FPS, int& m_totalFramesPassed)
{
	backBuffer.SetClearColour(0, 0, 0);

	//Draw entities
	//Player
	mx_player->Draw(backBuffer);
	m_wallController->Draw(backBuffer);
	
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
