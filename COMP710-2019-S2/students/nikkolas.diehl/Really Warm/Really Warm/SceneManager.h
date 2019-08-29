#pragma once
#include "iniParser.h"
#include "backbuffer.h"
#include "InGameScene.h"
#include "inputhandler.h"

//Libraries
#include <vector>

class SceneManager
{
public:
	SceneManager();
	~SceneManager();

	//Draw and process
	void Initialise(IniParser* m_iniParser, BackBuffer* m_pBackBuffer, int& screenWidth, int& screenHeight);
	void Draw(BackBuffer& m_pBackBuffer, int& m_FPS, int& m_totalFramesPassed);
	void Process(InputHandler* inputHandler, float deltaTime);

	//Game states
	enum State {
		MAIN_MENU,
		INGAME
	};
	//Current state
	State currentState;
	//Change state functionality
	void ChangeState(State stateToChange);
	//Scenes
	InGameScene* inGameScene;
};

