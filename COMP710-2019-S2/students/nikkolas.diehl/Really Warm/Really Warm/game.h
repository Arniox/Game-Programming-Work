// COMP710 GP 2D Framework 2019
#ifndef __GAME_H__
#define __GAME_H__

#include <vector>
#include "SceneManager.h"
#include "fmod.hpp"
#include "iniParser.h"

// Forward Declarations
class BackBuffer;
class InputHandler;
class Sprite;

class Game
{
	//Member Methods:
public:
	static Game& GetInstance();
	static void DestroyInstance();
	~Game();

	bool Initialise();
	bool DoGameLoop();
	void Quit();

	SceneManager* m_pSceneManager;

	//Ini parser
	IniParser* iniParser;

protected:
	void Process(float deltaTime);
	void Draw(BackBuffer& backBuffer);

private:
	Game(const Game& game);
	Game& operator=(const Game& game);
	
	Game();

	//Member Data:
public:
	//Main data
	int screenWidth;
	int screenHeight;
	bool fullScreen;

protected:
	static Game* sm_pInstance;
	BackBuffer* m_pBackBuffer;
	InputHandler* m_pInputHandler;
	bool m_looping;

	// Simulation Counters:
	float m_elapsedSeconds;
	float m_lag;
	float m_executionTime;
	__int64 m_lastTime;
	int m_frameCount;
	int m_totalFramesPassed;
	int m_FPS;
	int m_numUpdates;
	bool m_drawDebugInfo;

private:

};

#endif // __GAME_H__
