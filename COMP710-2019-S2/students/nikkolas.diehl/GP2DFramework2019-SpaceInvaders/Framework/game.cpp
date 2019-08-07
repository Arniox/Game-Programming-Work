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
const int screenWidth = 1280;
const int screenHeight = 720;

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

	//Empty the enemy container
	for (int i = 0; i < m_pEnemies.size(); i++) {
		delete m_pEnemies[i];
		m_pEnemies[i] = 0;
	}

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
	m_pBackBuffer = new BackBuffer();
	if (!m_pBackBuffer->Initialise(screenWidth, screenHeight))
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

	// SS04.4: Create the player ship instance.
	m_pPlayerShip = new PlayerShip;
	m_pPlayerShip->Initialise(m_pBackBuffer->CreateSprite("assets/Sprites/playership.png"));

	// SS04.5: Spawn four rows of 14 alien enemies.
	int columns = 14;
	int rows = 4;
	int entityCount = 0;
	
	//Set shift values for centering things
	float shiftX = (screenWidth / 2) - 700;
	float shiftY = 100;
	
	//For all enemies
	for (int i = 0; i < rows; i++) {
		for (int k = 0; k < columns; k++) {
			m_pEnemies.push_back(new Enemy());
			m_pEnemies.at(entityCount)->Initialise(m_pBackBuffer->CreateSprite("assets/Sprites/alienenemy.png"));
			m_pEnemies.at(entityCount)->SetCenter(k * 100 + shiftX, i * 100 + shiftY);

			entityCount++;
		}
	}

	//Set play position
	m_pPlayerShip->SetPositionX(screenWidth/2);
	m_pPlayerShip->SetPositionY(screenHeight-100);

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

	//For sound
	m_fmodSystem->update();

	// For Player
	m_pPlayerShip->Process(deltaTime, screenWidth, screenHeight, moveRight, moveLeft);

	//Process shooting
	if (shoot) {
		m_pBullets.push_back(new Bullet());
		m_pBullets.back()->Initialise(m_pBackBuffer->CreateSprite("assets/Sprites/playerbullet.png"));

		m_pBullets.back()->SetCenter(m_pPlayerShip->GetPositionX(), m_pPlayerShip->GetPositionY());
		shoot = false;

		//Play fire
		m_fmodSystem->playSound(sFireSound, 0, false, 0);
	}

	// Process each bullet in the container.
	float bulletVelocity = 150;
	for (int k = 0; k < m_pBullets.size(); k++) {
		float currentY = m_pBullets.at(k)->GetPositionY();
		m_pBullets.at(k)->SetPositionY(currentY - bulletVelocity * deltaTime);

		if (m_pBullets.at(k)->GetPositionY() < 15) {
			m_pBullets.at(k)->dead = true;
		}
	}
	// For each bullet
	for (int k = 0; k < m_pBullets.size(); k++) {
		m_pBullets.at(k)->Process(deltaTime);
	}

	// Process each alien enemy in the container.	
	for (int i = 0; i < m_pEnemies.size(); i++) {
		for (int k = 0; k < m_pBullets.size(); k++) {
			//If the two objects are right next to eachother (each is roughly 15 pixels big)
			float distance = sqrt(pow(m_pBullets.at(k)->GetPositionX() - m_pEnemies.at(i)->GetPositionX(), 2) + pow(m_pBullets.at(k)->GetPositionY() - m_pEnemies.at(i)->GetPositionY(), 2)) - 15 - 5;
			if (distance < 3) {
				//Delete at index
				m_pEnemies.at(i)->dead = true;
				m_pBullets.at(k)->dead = true;

				score++;

				//Play explosion
				m_fmodSystem->playSound(sExplosionSound, 0, false, 0);
			}
		}
	}

	// For each alien enemy
	for (int k = 0; k < m_pEnemies.size(); k++) {
		m_pEnemies.at(k)->Process(deltaTime);
	}

	//Delete bullets that are dead
	bulletIterator = m_pBullets.begin();
	while (bulletIterator != m_pBullets.end()) {
		if ((*bulletIterator)->dead) {
			delete (*bulletIterator);
			bulletIterator = m_pBullets.erase(bulletIterator);
		}
		else {
			bulletIterator++;
		}
	}

	//Delete enemies that are dead
	enemyIterator = m_pEnemies.begin();
	while (enemyIterator != m_pEnemies.end()) {
		if ((*enemyIterator)->dead) {
			delete (*enemyIterator);
			enemyIterator = m_pEnemies.erase(enemyIterator);
		}
		else {
			enemyIterator++;
		}
	}

	//If win state
	if (m_pEnemies.size() <= 0) {
		drawGame = false;
	}
}

void 
Game::Draw(BackBuffer& backBuffer)
{
	++m_frameCount;

	backBuffer.Clear();
	backBuffer.SetClearColour(0, 0, 0);

	if (drawGame) {
		// SS04.5: Draw all enemy aliens in container...
		for (int i = 0; i < m_pEnemies.size(); i++) {
			m_pEnemies.at(i)->Draw(backBuffer);
		}

		// SS04.6: Draw all bullets in container...
		for (int i = 0; i < m_pBullets.size(); i++) {
			m_pBullets.at(i)->Draw(backBuffer);
		}

		// SS04.4: Draw the player ship...
		m_pPlayerShip->Draw(backBuffer);
	}
	else {
		//Win State
	}
	
	//Draw Text
	backBuffer.SetTextColour(255, 0, 0);
	backBuffer.DrawText(20, 50, "Space Invaders!!!");

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