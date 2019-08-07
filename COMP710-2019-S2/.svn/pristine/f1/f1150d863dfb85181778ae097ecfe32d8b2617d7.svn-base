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
#include <cstdlib>
#include <ctime>
#include <math.h>
#include <iostream>

using namespace std;

// Static Members:
Game* Game::sm_pInstance = 0;
const int screenWidth = 1650;
const int screenHeight = 1050;

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
{
	
}

Game::~Game()
{
	delete m_pBackBuffer;
	m_pBackBuffer = 0;
}

bool 
Game::Initialise()
{
	//Set width and height

	srand(time(0));

	m_pBackBuffer = new BackBuffer();
	if (!m_pBackBuffer->Initialise(screenWidth, screenHeight))
	{
		LogManager::GetInstance().Log("BackBuffer Init Fail!");
		return (false);
	}

	m_pInputHandler = new InputHandler();
	if (!m_pInputHandler->Initialise())
	{
		LogManager::GetInstance().Log("InputHandler Init Fail!");
		return (false);
	}

	m_lastTime = SDL_GetPerformanceCounter();
	m_lag = 0.0f;

	m_pBackBuffer->SetClearColour(0xCC, 0xCC, 0xCC);

	//Entity stuff
	m_pBouncer = m_pBackBuffer->CreateSprite("assets/playership.png");
	m_pBouncer->SetX(1000);
	m_pBouncer->SetY(1000);
	angle = rand() % 360 + 1;

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
	float m_pVelocity = 150;
	float x = m_pBouncer->GetX();
	float y = m_pBouncer->GetY();

	//Bounce process
	if (x < 0 || x > (screenWidth-10)) {
		angle = 180 - angle;
		bounces++;
		cout << "Bounces: " << bounces << endl;
	}
	else if (y < 0 || y > (screenHeight-10)) {
		angle = 360 - angle;
		bounces++;
		cout << "Bounces: " << bounces << endl;
	}

	//Convert degrees to radian
	float angleDeg = angle * M_PI / 180;

	m_pBouncer->SetX(x + cosf(angleDeg) * m_pVelocity * deltaTime);
	m_pBouncer->SetY(y + sinf(angleDeg) * m_pVelocity * deltaTime);

	m_pBouncer->Process(deltaTime);


}

void 
Game::Draw(BackBuffer& backBuffer)
{
	++m_frameCount;

	backBuffer.Clear();

	//Get screenwidth and screenheight to place checkerboard in the center
	float xShift = (screenWidth / 2) - (500);
	float yShift = (screenHeight / 2) - (500);

	//Get the m_pBouncer sprites center point
	int bouncerX = m_pBouncer->GetX()+15;
	int bouncerY = m_pBouncer->GetY()+15;

	//Set up array for checkerboard box centers
	float boxCenters[20][20][2];

	//Draw the checkerboard if true
	if (drawCheckerboard) {
		for (int yDir = 0; yDir < 10; yDir++) {
			for (int xDir = 0; xDir < 10; xDir++) {
				if (yDir % 2 == 0) {
					if (xDir % 2 == 0) {
						//Set red every 2 boxes
						backBuffer.SetDrawColour(255, 0, 0);
					}
					else {
						//Otherwise set green
						backBuffer.SetDrawColour(0, 255, 0);
					}
				}
				else {
					if (xDir % 2 == 0) {
						//Otherwise set green
						backBuffer.SetDrawColour(0, 255, 0);
					}
					else {
						//Set red every 2 boxes
						backBuffer.SetDrawColour(255, 0, 0);
					}
				}

				backBuffer.DrawRectangle(xShift + (xDir * 100), yShift + (yDir * 100), xShift + (xDir * 100 + 100), yShift + (yDir * 100 + 100));

				boxCenters[yDir][xDir][0] = (xShift + (xDir * 100)) + 50;
				boxCenters[yDir][xDir][1] = (yShift + (yDir * 100)) + 50;
			}
		}
	}
		
	//Set black for drawing lines
	if (drawLines && drawCheckerboard) {
		backBuffer.SetDrawColour(0, 0, 0);
		for (int i = 0; i < 10; ++i) {
			for (int k = 0; k < 10; ++k) {
				backBuffer.DrawLine(boxCenters[i][k][0], boxCenters[i][k][1], bouncerX, bouncerY);
			}
		}
	}

	//If Draw Sprite
	if (drawSprite) {
		m_pBouncer->Draw(backBuffer);
	}

	backBuffer.Present();
}

void 
Game::Quit()
{
	m_looping = false;
}


