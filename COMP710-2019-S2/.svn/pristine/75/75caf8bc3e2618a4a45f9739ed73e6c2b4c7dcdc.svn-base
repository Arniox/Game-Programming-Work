// COMP710 GP 2D Framework 2019
#ifndef __GAME_H__
#define __GAME_H__

#include <vector>
#include "PlayerShip.h"
#include "Bullet.h"
#include "Enemy.h"
#include "fmod.hpp"
#include "EnemyPool.h"
#include "BulletPool.h"

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

	void InitFMOD();

	void UnloadFMOD();

	//Sounds
	FMOD::Sound* sFireSound;
	FMOD::Sound* sExplosionSound;
	FMOD::Sound* sMoveSound;

protected:
	void Process(float deltaTime);
	void Draw(BackBuffer& backBuffer);

private:
	Game(const Game& game);
	Game& operator=(const Game& game);
	
	Game();

	//Member Data:
public:
	//Entity data
	bool shoot = false;
	bool moveRight = false;
	bool moveLeft = false;
	bool drawGame = true;

	int score;

	std::vector<Bullet*>::iterator bulletIterator;

	//Fmod
	FMOD::System* m_fmodSystem;
	FMOD::Channel* m_fmodChannel;

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
	int m_FPS;
	int m_numUpdates;
	bool m_drawDebugInfo;
	
	// Game Entities:
	EnemyPool* m_enemyPool;
	BulletPool* m_bulletPool;
	PlayerShip* m_pPlayerShip;

private:

};

#endif // __GAME_H__
