#pragma once
#include "iniParser.h"
#include "backbuffer.h"
#include "InGameScene.h"
#include <vector>

class SceneManager
{
public:
	SceneManager();
	~SceneManager();

	//Draw and process
	void SceneManager::Initialise(IniParser* m_iniParser, BackBuffer* m_pBackBuffer, int& screenWidth, int& screenHeight);
	void SceneManager::Draw(BackBuffer& m_pBackBuffer, int& m_FPS, int& m_totalFramesPassed);
	void SceneManager::Process(float deltaTime);

	//Game states
	enum State {
		MAIN_MENU,
		INGAME
	};
	//Current state
	State currentState;
	//Change state functionality
	void SceneManager::ChangeState(State stateToChange);
	//Scenes
	InGameScene* inGameScene;
};

