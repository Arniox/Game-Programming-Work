#include "SceneManager.h"

#include "IniParser.h"
#include "backbuffer.h"
#include "game.h"

#include "SplashScreen.h"
#include "MainMenu.h"
#include "InstructionMenu.h"
#include "Level.h"
#include "Test.h"
#include "Test2.h"
#include "Test51.h"
#include "CarTest.h"
#include "CarTest2.h"
#include "Camera.h"
#include "FontManager.h"
#include "GUI.h"

SceneManager::SceneManager()
: m_pCurrentState(nullptr)
{

}

SceneManager::~SceneManager()
{
	if (m_pCurrentState != nullptr)
	{
		delete m_pCurrentState;
		m_pCurrentState = nullptr;
	}

	delete m_pStatsGUI;
	m_pStatsGUI = nullptr;
}

void 
SceneManager::Initialise(SCENES startScene)
{
	//Copy references
	m_pIniFile = Game::GetIniFile();
	m_pBackBuffer = Game::GetBackBuffer();
	m_pFontManager = Game::GetFontManager();
	SetNextState(startScene);
	Game::GetCamera()->SetPos(0, 0);
	ChangeState();

	m_pStatsGUI = new GUI();

	m_pStatsGUI->AddHUDElement("Surface Count: ", H_BLOCK, 10, 10, LEFT_TOP, 0.4, CENTER_LEFT);
	m_pStatsGUI->AddHUDElement("FPS: ", H_BLOCK, 10, 35, LEFT_TOP, 0.4, CENTER_LEFT);
}

void
SceneManager::Draw(BackBuffer& backBuffer)
{
	m_pCurrentState->Draw(backBuffer);

	int surfaceCount = backBuffer.GetRenderCount();
	int fps = Game::GetFPS();

	m_pStatsGUI->UpdateHud(0, ("Surface Count: " + std::to_string(surfaceCount)).c_str());
	m_pStatsGUI->UpdateHud(1, ("FPS: " + std::to_string(fps)).c_str());

	m_pStatsGUI->Draw(backBuffer);
}

void
SceneManager::Process(float deltaTime)
{
	m_pStatsGUI->Process(deltaTime);
	m_pCurrentState->Proccess(deltaTime);
}

void 
SceneManager::ChangeState()
{
	if (m_nextSceneID != SCENE_NULL)
	{
		State* pOldState = m_pCurrentState;
		m_pBackBuffer->Clear();

		switch (m_nextSceneID)
		{
		case SCENE_NULL:
			break;
		case SCENE_SPLASH_AUT:
			m_pCurrentState = new SplashScreen("splashLogoAUT", SCENE_SPLASH_FMOD);
			break;
		case SCENE_SPLASH_FMOD:
			m_pCurrentState = new SplashScreen("splashLogoFMOD", SCENE_MAIN_MENU);
			break;
		case SCENE_MAIN_MENU:
			m_pCurrentState = new MainMenu();
			break;
		case SCENE_INSTRUCTIONS:
			m_pCurrentState = new InstructionMenu();
			break;
		case SCENE_LEVEL:
			m_pCurrentState = new Level();
			break;
		case SCENE_TEST:
			m_pCurrentState = new Test();
			break;
		case SCENE_TEST2:
			m_pCurrentState = new Test2();
			break;
		case SCENE_CAR_TEST:
			m_pCurrentState = new CarTest2();
			break;	
		case SCENE_CAR_TEST2:
			m_pCurrentState = new CarTest2();
			break;
		case SCENE_TEST51:
			m_pCurrentState = new Test51();
			break;	
		default:
			break;
		}

		Game::GetCamera()->SetPos(0, 0);
		m_sceneID = m_nextSceneID;
		m_nextSceneID = SCENE_NULL;

		if (m_pCurrentState != nullptr)
		{
			delete pOldState;
			pOldState = nullptr;
		}
	}
}

void
SceneManager::SetNextState(SCENES scene)
{
	m_nextSceneID = scene;
}

void
SceneManager::ProcessControls()
{
	m_pCurrentState->ProcessControls();
}
