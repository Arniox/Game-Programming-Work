#pragma once
#include "backbuffer.h"
#include "iniParser.h"
#include "fmod.hpp"
#include "Player.h"
#include "Walls.h"
#include "inputhandler.h"

class InGameScene
{
public:
	InGameScene();
	~InGameScene();

	//Main functionality
	void Initialise(IniParser* m_iniParser, BackBuffer* m_pBackBuffer, int& screenWidth, int& screenHeight);
	void Draw(BackBuffer& backBuffer, int& m_FPS, int& m_totalFramesPassed);
	void Process(InputHandler* ep_inputHander, float deltaTime);

private:
	//Fmod
	FMOD::System* m_fmodSystem;
	FMOD::Channel* m_fmodChannel;

	//Ingame members
	Walls* wallsTest;
	Player* mx_player;

protected:
	//Main
	int m_windowWidth;
	int m_windowHeight;

	void UnloadFMOD();
	void InitFMOD();
};

