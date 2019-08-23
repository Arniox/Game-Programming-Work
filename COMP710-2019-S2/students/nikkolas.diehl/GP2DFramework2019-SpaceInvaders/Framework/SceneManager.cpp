#include "SceneManager.h"



SceneManager::SceneManager()
:currentState(INGAME)
,inGameScene()
{
}

SceneManager::~SceneManager()
{
	delete inGameScene;
	inGameScene = 0;
}

void SceneManager::Initialise(IniParser* m_iniParser, BackBuffer* m_pBackBuffer, int& screenWidth, int& screenHeight)
{
	inGameScene = new InGameScene();
	inGameScene->Initialise(m_iniParser, m_pBackBuffer, screenWidth, screenHeight);
}

void
SceneManager::Process(float deltaTime)
{
	switch (currentState) {
	case(MAIN_MENU):

		break;
	case(INGAME):
		inGameScene->Process(deltaTime);
		break;
	}
}

void
SceneManager::Draw(BackBuffer& m_backBuffer, int& m_FPS, int& m_totalFramesPassed)
{
	switch (currentState) {
		case(MAIN_MENU):

			break;
		case(INGAME):
			inGameScene->Draw(m_backBuffer, m_FPS, m_totalFramesPassed);
			break;
	}
}

void 
SceneManager::ChangeState(State stateToChange)
{
	currentState = stateToChange;
}
