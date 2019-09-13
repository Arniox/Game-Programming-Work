#pragma once

//Locals
#include "backbuffer.h"
#include "iniParser.h"
#include "fmod.hpp"
#include "Player.h"
#include "Walls.h"
#include "WallController.h"
#include "inputhandler.h"
#include "QuadTree.h"
#include "Rectangle.h"
#include "inputhandler.h"
#include "PhysicsEngine.h"

//Libraries
#include <time.h>

class InGameScene
{
public:
	InGameScene();
	~InGameScene();

	//Main functionality
	void Initialise(IniParser* m_iniParser, BackBuffer* m_pBackBuffer, int screenWidth, int screenHeight);
	void Draw(BackBuffer& backBuffer, int& m_FPS, int& m_totalFramesPassed);
	void Process(InputHandler* ep_inputHander, float deltaTime, int screenWidth, int screenHeight);

	void InsertAll();

private:
	//Fmod
	FMOD::System* m_fmodSystem;
	FMOD::Channel* m_fmodChannel;

	//Ingame members
	WallController* m_wallController;
	Player* mx_player;

protected:
	//Quad Tree functionality
	Rectangle* screenBoundry;
	//QuadTree* qt;
	int totalEntityCount;

	//Physics engine
	PhysicsEngine* physicsEngine;

	//Main
	int m_windowWidth;
	int m_windowHeight;

	void UnloadFMOD();
	void InitFMOD();
};

