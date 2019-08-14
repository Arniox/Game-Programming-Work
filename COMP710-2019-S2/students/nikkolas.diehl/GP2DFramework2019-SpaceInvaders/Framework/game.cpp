// COMP710 GP 2D Framework 2019

// This includes:
#include "game.h"

// Local includes:
#include "backbuffer.h"
#include "inputhandler.h"
#include "logmanager.h"
#include "sprite.h"

// Library includes:
#include <cassert>
#include <SDL.h>
#include <cstdio>

// Name spacing
using namespace std;

// Static Members:
Game* Game::sm_pInstance = 0;

Game&
Game::GetInstance()
{
	if (sm_pInstance == 0)
	{ 
		sm_pInstance = new Game();
	}

	assert(sm_pInstance);

	return (*sm_pInstance);
}

void 
Game::DestroyInstance()
{
	delete sm_pInstance;
	sm_pInstance = 0;
}

Game::Game()
: m_pBackBuffer(0)
, m_pInputHandler(0)
, m_looping(true)
, m_executionTime(0)
, m_elapsedSeconds(0)
, m_frameCount(0)
, m_FPS(0)
, m_numUpdates(0)
, m_lastTime(0)
, m_lag(0)
, score(0)
{
	
}

Game::~Game()
{
	delete m_pBackBuffer;
	m_pBackBuffer = 0;

	//Delete the enemy pool
	delete m_enemyPool;
	m_enemyPool = 0;

	//Empty the bullet pool
	delete m_bulletPool;
	m_bulletPool = 0;

	//Empty the explosion pool
	delete m_explosionPool;
	m_explosionPool = 0;

	//Delete explosion coords
	m_explosionCoords.empty();

	//Empty sound
	UnloadFMOD();

	//Empty player
	delete m_pPlayerShip;
	m_pPlayerShip = 0;

	delete m_pInputHandler;
	m_pInputHandler = 0;

	delete m_pBackBuffer;
	m_pBackBuffer = 0;
}

bool 
Game::Initialise()
{
	//--------------File Parser---------------------
	iniParser = new IniParser();
	screenWidth = iniParser->GetValueAsInt("Main", "windowWidth");
	screenHeight = iniParser->GetValueAsInt("Main", "windowHeight");
	fullScreen = iniParser->GetValueAsBoolean("Main", "fullScreen");


	m_pBackBuffer = new BackBuffer();
	if (!m_pBackBuffer->Initialise(screenWidth, screenHeight, fullScreen))
	{
		LogManager::GetInstance().Log("BackBuffer Init Fail!");
		return (false);
	}

	//Init Fmod
	InitFMOD();

	m_pInputHandler = new InputHandler();
	if (!m_pInputHandler->Initialise())
	{
		LogManager::GetInstance().Log("InputHandler Init Fail!");
		return (false);
	}

	m_lastTime = SDL_GetPerformanceCounter();
	m_lag = 0.0f;

	m_pBackBuffer->SetClearColour(0xCC, 0xCC, 0xCC);

	//--------------Member data---------------------

	//Set shift values for centering things
	int rows = 4;
	int columns = 14;
	float shiftX = (screenWidth / 2) - ((columns / 2)*(screenWidth/10));
	float shiftY = (screenHeight/10);

	//Create the player ship instance.
	m_pPlayerShip = new PlayerShip;
	m_pPlayerShip->Initialise(m_pBackBuffer->CreateSprite("assets/Sprites/playership.png"));
	//Set play position
	m_pPlayerShip->SetPositionX(screenWidth / 2);
	m_pPlayerShip->SetPositionY(screenHeight - (screenHeight)/10);
	
	//Init the enemy pool
	m_enemyPool = new EnemyPool();
	m_enemyPool->Initialise(rows, columns, m_pBackBuffer, shiftX, shiftY, screenWidth, screenHeight);
	//Init the bullet pool
	m_bulletPool = new BulletPool();
	m_bulletPool->Initialise(m_pBackBuffer);
	//Init the explosion pool
	m_explosionPool = new ExplosionPool();
	m_explosionPool->Initialise(m_pBackBuffer);

	return (true);
}

bool 
Game::DoGameLoop()
{
	const float stepSize = 1.0f / 60.0f;

	assert(m_pInputHandler);
	m_pInputHandler->ProcessInput(*this);
	
	if (m_looping)
	{
		Uint64 current = SDL_GetPerformanceCounter();

		float deltaTime = (current - m_lastTime) / static_cast<float>(SDL_GetPerformanceFrequency());
		
		m_lastTime = current;

		m_executionTime += deltaTime;

		Process(deltaTime);
	
		m_lag += deltaTime;

		int innerLag = 0;

		while (m_lag >= stepSize)
		{
			Process(stepSize);

			m_lag -= stepSize;

			++m_numUpdates;
			++innerLag;
		}

		// DEBUG STUB:
//		char buffer[64];
//		sprintf(buffer, "%f", deltaTime);
//		LogManager::GetInstance().Log(buffer);

		Draw(*m_pBackBuffer);
	}

	return (m_looping);
}

void 
Game::Process(float deltaTime)
{
	// Count total simulation time elapsed:
	m_elapsedSeconds += deltaTime;

	// Frame Counter:
	if (m_elapsedSeconds > 1)
	{
		m_elapsedSeconds -= 1;
		m_FPS = m_frameCount;
		m_frameCount = 0;
	}

	// Update the game world simulation:
	float bulletX = 0;
	float bulletY = 0;
	float enemyX = 0;
	float enemyY = 0;
	// Process death collision
	for (int i = 0; i < m_enemyPool->GetEnemies().size(); i++) {
		for (int k = 0; k < m_bulletPool->GetBullets().size(); k++) {
			//If the two objects are right next to eachother (each is roughly 15 pixels big)
			bulletX = m_bulletPool->GetBulletAt(k)->GetPositionX();
			bulletY = m_bulletPool->GetBulletAt(k)->GetPositionY();
			enemyX = m_enemyPool->GetEnemyAt(i)->GetPositionX();
			enemyY = m_enemyPool->GetEnemyAt(i)->GetPositionY();
			float distance = sqrt(pow(bulletX - enemyX, 2) + pow(bulletY - enemyY, 2)) - 15 - 5;
			if (distance < 3) {
				//Delete at index
				m_enemyPool->GetEnemyAt(i)->dead = true;
				m_bulletPool->GetBulletAt(k)->dead = true;
				explode = true;
				//Save the x and y coords of all the explosions
				m_explosionCoords.push_back(new std::vector<int>);
				m_explosionCoords.back()->push_back(enemyX-10);
				m_explosionCoords.back()->push_back(enemyY-10);

				score++;
			}
		}
	}

	//Process enemies
	m_enemyPool->Process(deltaTime);
	//Process bullets
	m_bulletPool->Process(m_fmodSystem, sFireSound, deltaTime, shoot, m_pPlayerShip->GetPositionX(), m_pPlayerShip->GetPositionY());
	//Process Explosions
	m_explosionPool->Process(m_fmodSystem, sExplosionSound, deltaTime, explode, m_explosionCoords);
	//Process Player
	m_pPlayerShip->Process(deltaTime, screenWidth, screenHeight, moveRight, moveLeft);
	//Process sound
	m_fmodSystem->update();
}

void 
Game::Draw(BackBuffer& backBuffer)
{
	++m_frameCount;
	++m_totalFramesPassed;

	backBuffer.Clear();
	backBuffer.SetClearColour(0, 0, 0);

	if (drawGame) {
		//Draw enemies in enemy pool
		m_enemyPool->Draw(backBuffer);
		//Draw all bullets in container...
		m_bulletPool->Draw(backBuffer);
		//Draw all explosions
		m_explosionPool->Draw(backBuffer);

		//Draw the player ship...
		m_pPlayerShip->Draw(backBuffer);
	}
	else {
		//Win State
	}
	
	//Draw Text for title
	backBuffer.SetTextColour(255, 255, 255);
	string titleOutPut = "Space Invaders! Score: " + to_string(score);
	backBuffer.DrawText(20, 50, titleOutPut.c_str());

	//Draw Text for fps
	backBuffer.SetTextColour(255, 255, 255);
	string fps = "Fps: " + to_string(m_FPS) + " | frameCount: " + to_string(m_totalFramesPassed);
	backBuffer.DrawText(screenWidth/2, 50, fps.c_str());

	backBuffer.Present();
}

void 
Game::Quit()
{
	m_looping = false;
}

void
Game::InitFMOD()
{
	FMOD::System_Create(&m_fmodSystem);
	m_fmodSystem->init(32, FMOD_INIT_NORMAL, 0);

	m_fmodSystem->createSound("assets/Sounds/Shoot.wav", FMOD_DEFAULT, NULL, &sFireSound);
	m_fmodSystem->createSound("assets/Sounds/Explosion.wav", FMOD_DEFAULT, NULL, &sExplosionSound);
	m_fmodSystem->createSound("assets/Sounds/Move.wav", FMOD_DEFAULT, NULL, &sMoveSound);
}

void
Game::UnloadFMOD() 
{
	sFireSound->release();
	sExplosionSound->release();
	sMoveSound->release();
}
