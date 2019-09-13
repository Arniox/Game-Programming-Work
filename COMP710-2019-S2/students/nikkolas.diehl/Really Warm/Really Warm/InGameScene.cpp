#include "InGameScene.h"

InGameScene::InGameScene()
:mx_player(0)
{
}
InGameScene::~InGameScene()
{
	//Empty sound
	UnloadFMOD();

	delete m_wallController;
	m_wallController = 0;

	delete mx_player;
	mx_player = 0;

	delete physicsEngine;
	physicsEngine = 0;

	//delete qt;
	//qt = 0;
}

void 
InGameScene::Initialise(IniParser* m_iniParser, BackBuffer* m_pBackBuffer, int screenWidth, int screenHeight)
{
	//Init Fmod
	InitFMOD();

	//--------------Member data---------------------
	m_windowWidth = screenWidth;
	m_windowHeight = screenHeight;

	physicsEngine = new PhysicsEngine();

	//--------------Ingame entities-----------------
	mx_player = new Player();
	mx_player->CreatePlayer(m_pBackBuffer, (static_cast<double>(screenWidth) / 2.0) - 10.0, 100.0);

	//--------------Walls---------------------------
	m_wallController = new WallController();
	m_wallController->Initialise(m_pBackBuffer, static_cast<double>(screenWidth), static_cast<double>(screenHeight));

	//--------------Quad Tree-----------------------
	//qt = new QuadTree(screenWidth, screenHeight, 16, 16);
}

void
InGameScene::Process(InputHandler* ep_inputHander, float deltaTime, int screenWidth, int screenHeight)
{
	//Process sound
	m_fmodSystem->update();

	//Process entities
	//Player
	bool test = ep_inputHander->GetKeyBoardLayout("d");
	mx_player->Process(ep_inputHander);

	//Physics
	//-------------------Process the player--------------------
	physicsEngine->ProcessEntity(deltaTime, mx_player, m_wallController->GetWalls(), screenWidth, screenHeight);

	//Insert all objects into quadtree
	//InsertAll();

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
