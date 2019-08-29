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
#include <time.h>

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
, m_totalFramesPassed(0)
{
	
}

Game::~Game()
{
	delete m_pSceneManager;
	m_pSceneManager = nullptr;

	delete iniParser;
	iniParser = nullptr;

	delete m_pBackBuffer;
	m_pBackBuffer = nullptr;
	
	delete m_pInputHandler;
	m_pInputHandler = nullptr;

	delete m_pBackBuffer;
	m_pBackBuffer = nullptr;
}

bool 
Game::Initialise()
{
	//Seed the srand
	srand((unsigned int)time(0));

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
	m_pSceneManager = new SceneManager();
	m_pSceneManager->Initialise(iniParser, m_pBackBuffer, screenWidth, screenHeight);

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

		//Process(deltaTime);
	
		m_lag += deltaTime;

		int innerLag = 0;

		while (m_lag >= stepSize)
		{
			Process(stepSize);

			m_lag -= stepSize;

			++m_numUpdates;
			++innerLag;
		}

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

	//Process Scene manager
	m_pSceneManager->Process(m_pInputHandler, deltaTime);
}

void 
Game::Draw(BackBuffer& backBuffer)
{
	++m_frameCount;
	++m_totalFramesPassed;

	

	backBuffer.Clear();

	if (m_pInputHandler->GetKeyBoardLayout("r")) {
		backBuffer.SetClearColour(255, 0, 0);
	}
	else if (m_pInputHandler->GetKeyBoardLayout("g")) {
		backBuffer.SetClearColour(0, 255, 0);
	}
	else if (m_pInputHandler->GetKeyBoardLayout("b")) {
		backBuffer.SetClearColour(0, 0, 255);
	}
	else {
		backBuffer.SetClearColour(30, 30, 30);
	}

	//Draw Scene manager
	m_pSceneManager->Draw(backBuffer, m_FPS, m_totalFramesPassed);

	backBuffer.Present();
}

void 
Game::Quit()
{
	m_looping = false;
}
