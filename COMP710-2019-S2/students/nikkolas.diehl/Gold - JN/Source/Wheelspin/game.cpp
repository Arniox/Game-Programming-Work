#define _CRTDBG_MAP_ALLOC

// This includes:
#include "game.h"

// Local includes:
#include "backbuffer.h"
#include "InputManager.h"
#include "SceneManager.h"
#include "logmanager.h"
#include "IniParser.h"
#include "FontManager.h"
#include "SoundManager.h"
#include "Constants.h"
#include "Camera.h"

// Library includes:
#include <cassert>
#include <SDL.h>
#include <cstdio>
#include <time.h>
#include <stdlib.h>
//#include <Box2D\Box2D.h>

// Static Members:
Game* Game::sm_pInstance = 0;

Game*
Game::GetInstance()
{
	if (sm_pInstance == 0)
	{
		sm_pInstance = new Game();
	}

	assert(sm_pInstance);

	return (sm_pInstance);
}

void 
Game::DestroyInstance()
{
	delete sm_pInstance; 
	sm_pInstance = 0;
}

Game::Game()
: m_pBackBuffer(nullptr)
, m_pInputManager(nullptr)
, m_pSceneManager(nullptr)
, m_pIniFile(nullptr)
, m_pFontManager(nullptr)
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
	delete m_pBackBuffer;
	m_pBackBuffer = nullptr;

	delete m_pInputManager;
	m_pInputManager = nullptr;

	delete m_pSceneManager;
	m_pSceneManager = nullptr;

	delete m_pIniFile;
	m_pIniFile = nullptr;

	delete m_pFontManager;
	m_pFontManager = nullptr;

	delete m_pCamera;
	m_pCamera = nullptr;

	delete m_pSoundManager;
	m_pSoundManager = nullptr;
}

bool 
Game::Initialise()
{
	//Seed the srand
	srand(static_cast<unsigned int>(time(0)));

	// Create and initialise a log manager singleton
	LogManager::GetInstance();

	//--------------File Parser---------------------
	m_pIniFile = new IniParser();
	m_pIniFile->LoadIniFile("settings.ini");

	LogManager::GetInstance().SetDebugLogging(m_pIniFile->GetValueAsBoolean("debug", "debugLogging"));
	LogManager::Title("Main Window Data");

	m_screenWidth = m_pIniFile->GetValueAsInt("Main", "windowWidth");
	m_screenHeight = m_pIniFile->GetValueAsInt("Main", "windowHeight");
	m_fullScreen = m_pIniFile->GetValueAsBoolean("Main", "fullScreen");

	float M2P = m_pIniFile->GetValueAsFloat("debug", "metersToPixels");
	float P2M = 1.0f / M2P;

	LogManager::Title("Camera Data");
	m_pCamera = new Camera(M2P, P2M);

	m_pBackBuffer = new BackBuffer();

	LogManager::Title("BackBuffer Intialisation");
	if (!m_pBackBuffer->Initialise(m_screenWidth, m_screenHeight, m_fullScreen, M2P, P2M))
	{
		LogManager::Debug("BackBuffer Init Fail!");
		LogManager::GetInstance().DestroyInstance();
		return (false);
	}

	LogManager::Title("Input Manager");
	m_pInputManager = new InputManager();
	if (!m_pInputManager->Initialise())
	{

		LogManager::Debug("InputHandler Init Fail!");
		LogManager::GetInstance().DestroyInstance();
		return (false);
	}

	LogManager::Title("Font Manager");
	m_pFontManager = new FontManager();
	if (!m_pFontManager->Initialise(m_pIniFile)) {
		LogManager::Debug("FontManager Init Fail!");
		LogManager::GetInstance().DestroyInstance();
		return (false);
	}

	m_lastTime = SDL_GetPerformanceCounter();
	m_lag = 0.0f;

	m_pBackBuffer->SetClearColour(0x51, 0x51, 0x51);

	//Init scene manager
	LogManager::Title("Scene Manager");
	m_pSceneManager = new SceneManager();
	m_pSceneManager->Initialise(static_cast<SCENES>(m_pIniFile->GetValueAsInt("Game", "startState")));

	//Init sound manager
	LogManager::Title("Sound Manager");
	m_pSoundManager = new SoundManager();
	m_pSoundManager->Initialise(m_pIniFile);
	LogManager::Title("Game Setup Complete!");

	return (true);
}

bool 
Game::DoGameLoop()
{
	const float stepSize = 1.0f / 60.0f;

	assert(m_pInputManager);
	m_pInputManager->Proccess();
	
	if (m_looping)
	{
		Uint64 current = SDL_GetPerformanceCounter();
		float deltaTime = (current - m_lastTime) / static_cast<float>(SDL_GetPerformanceFrequency());
		m_lastTime = current;
		m_executionTime += deltaTime;
	
		m_lag += deltaTime;
		int innerLag = 0;

		m_pSceneManager->ProcessControls();

		while (m_lag >= stepSize)
		{
			Process(stepSize);

			m_lag -= stepSize;
			++m_numUpdates;
			++innerLag;
		}

		m_pSceneManager->ChangeState();

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
	if (m_elapsedSeconds > 1.0f)
	{
		m_elapsedSeconds -= 1.0f;
		m_FPS = m_frameCount;
		m_frameCount = 0;
	}

	//Process scene manager
	m_pSceneManager->Process(deltaTime);
}

void 
Game::Draw(BackBuffer& backBuffer)
{
	++m_frameCount;
	++m_totalFramesPassed;

	backBuffer.Clear();

	m_pSceneManager->Draw(backBuffer);

	backBuffer.Present();
}

void 
Game::Quit()
{
	m_looping = false;
}

IniParser*
Game::GetIniFile()
{
	return Game::GetInstance()->m_pIniFile;
}

FontManager*
Game::GetFontManager()
{
	return Game::GetInstance()->m_pFontManager;
}

SoundManager*
Game::GetSoundManager()
{
	return Game::GetInstance()->m_pSoundManager;
}

Camera* 
Game::GetCamera()
{
	return Game::GetInstance()->m_pCamera;
}

BackBuffer*
Game::GetBackBuffer()
{
	return Game::GetInstance()->m_pBackBuffer;
}

InputManager*
Game::GetInputManager()
{
	return Game::GetInstance()->m_pInputManager;
}

SceneManager* 
Game::GetSceneManager()
{
	return Game::GetInstance()->m_pSceneManager;
}

int
Game::GetScreenWidth()
{
	return Game::GetInstance()->m_screenWidth;
}

int
Game::GetScreenHeight()
{
	return Game::GetInstance()->m_screenHeight;
}

int
Game::GetFPS()
{
	return Game::GetInstance()->m_FPS;
}