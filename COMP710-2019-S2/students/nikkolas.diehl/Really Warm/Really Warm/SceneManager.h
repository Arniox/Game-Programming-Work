#pragma once

#include "iniParser.h"
#include "backbuffer.h"
#include "InGameScene.h"
#include "SplashScreen.h"
#include "inputhandler.h"

//Libraries
#include <vector>

class SceneManager
{
public:
	SceneManager();
	~SceneManager();

	//Draw and process
	void Initialise(IniParser* m_iniParser, BackBuffer* m_pBackBuffer, int screenWidth, int screenHeight);
	void Draw(BackBuffer& m_pBackBuffer, int& m_FPS, int& m_totalFramesPassed);
	void Process(InputHandler* inputHandler, float deltaTime, int screenWidth, int screenHeight);

	//Game states
	enum State {
		SPLASH_SCREEN,
		MAIN_MENU,
		INGAME
	};
	//Current state
	State currentState;
	//Change state functionality
	void ChangeState(State stateToChange);
	//Scenes
	InGameScene* inGameScene;
	SplashScreen* splashScreenScene;

private:
	//Timing
	const int SplashMax = 3;
	double splashScreenTime = 0;
};

