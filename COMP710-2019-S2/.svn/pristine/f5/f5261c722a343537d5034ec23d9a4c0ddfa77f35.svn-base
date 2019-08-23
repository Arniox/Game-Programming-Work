#pragma once
#include "backbuffer.h"
#include "iniParser.h"
#include "fmod.hpp"
#include "PlayerShip.h"
#include "EnemyPool.h"
#include "BulletPool.h"
#include "ExplosionPool.h"
#include "ParticleManager.h"

class InGameScene
{
public:
	InGameScene();
	~InGameScene();

	//Main functionality
	void InGameScene::Initialise(IniParser* m_iniParser, BackBuffer* m_pBackBuffer, int& screenWidth, int& screenHeight);
	void InGameScene::Draw(BackBuffer& backBuffer, int& m_FPS, int& m_totalFramesPassed);
	void InGameScene::Process(float deltaTime);

	//Setters
	void InGameScene::SetShoot(bool shoot);
	void InGameScene::SetMoveRight(bool right);
	void InGameScene::SetMoveLeft(bool left);

	//Sounds
	FMOD::Sound* sFireSound;
	FMOD::Sound* sExplosionSound;
	FMOD::Sound* sMoveSound;

private:
	//Outputs
	string* titleOutPut;

	//Entity data
	bool shoot = false;
	bool explode = false;
	bool moveRight = false;
	bool moveLeft = false;
	bool drawGame = true;
	int score;

	//Fmod
	FMOD::System* m_fmodSystem;
	FMOD::Channel* m_fmodChannel;

protected:
	// Game Entities:
	EnemyPool* m_enemyPool;
	BulletPool* m_bulletPool;
	ExplosionPool* m_explosionPool;
	ParticleManager* m_paritcleManager;

	std::vector<std::vector<int>*> m_explosionCoords;

	PlayerShip* m_pPlayerShip;

	//Main
	int m_windowWidth;
	int m_windowHeight;

	void InGameScene::UnloadFMOD();
	void InGameScene::InitFMOD();
};

