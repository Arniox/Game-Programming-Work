#include "SceneManager.h"



SceneManager::SceneManager()
: currentState(SPLASH_SCREEN)
, inGameScene()
, splashScreenScene()
{
}

SceneManager::~SceneManager()
{
	delete inGameScene;
	inGameScene = 0;
	delete splashScreenScene;
	splashScreenScene = 0;
}

void SceneManager::Initialise(IniParser* m_iniParser, BackBuffer* m_pBackBuffer, int screenWidth, int screenHeight)
{
	splashScreenScene = new SplashScreen();
	splashScreenScene->Initialise(m_pBackBuffer, screenWidth, screenHeight);

	inGameScene = new InGameScene();
	inGameScene->Initialise(m_iniParser, m_pBackBuffer, screenWidth, screenHeight);
}

void
SceneManager::Process(InputHandler* inputHandler, float deltaTime, int screenWidth, int screenHeight)
{
	switch (currentState) {
	case(SPLASH_SCREEN):
		splashScreenTime += deltaTime;
		if (static_cast<int>(splashScreenTime) >= SplashMax) {
			ChangeState(INGAME);
		}
		break;
	case(MAIN_MENU):

		break;
	case(INGAME):
		inGameScene->Process(inputHandler, deltaTime, screenWidth, screenHeight);
		break;
	}
}

void
SceneManager::Draw(BackBuffer& m_backBuffer, int& m_FPS, int& m_totalFramesPassed)
{
	switch (currentState) {
		case(SPLASH_SCREEN):
			splashScreenScene->Draw(m_backBuffer);
			break;
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
