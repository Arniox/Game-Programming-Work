#include "Level.h"

#include "game.h"
#include "IniParser.h"
#include "DebugDraw.h"
#include "logmanager.h"
#include "Camera.h"
#include "Wheel.h"
#include "Car.h"
#include "backbuffer.h"
#include "sprite.h"
#include "Calc.h"
#include "DebugSquare.h"
#include "LevelsManager.h"
#include "Arrow.h"
#include "GUI.h"
#include "SceneManager.h"

Level::Level()
: m_carControl(0)
, m_accelerator(0.0f)
, m_turnAmount(0.0f)
, m_debugDraw(false)
, m_pDebugDrawer(nullptr)
, m_pBackBuffer(nullptr)
, m_pCam(nullptr)
, m_pCar(nullptr)
, m_pLevelsManager(nullptr)
, m_pPauseGUI(nullptr)
, m_paused(false)
, m_pStatsGUI(nullptr)
, m_pWinScreen(nullptr)
, m_timeTaken(0.0f)
, m_distanceToFinish(0.0f)
, m_gameWon(false)
{
	LogManager::Title("Ingame Scene");

	m_pBackBuffer = Game::GetBackBuffer();
	m_pInput = Game::GetInputManager();
	m_pCam = Game::GetCamera();

	M2P = Game::GetIniFile()->GetValueAsFloat("debug", "metersToPixels");
	P2M = 1.0f / M2P;

	SetupWorld();
	m_pArrow = new Arrow(0, static_cast<int>(-200 * M2P));

	m_debugDraw = Game::GetIniFile()->GetValueAsBoolean("debug", "debugDrawing");

	m_pDebugDrawer = new DebugDraw(M2P, P2M);
	if (!m_pDebugDrawer->Init(m_pWorld, m_pBackBuffer))
		LogManager::Debug("ERROR: Unable to init debug drawer");

	//Levels testing
	LogManager::Title("Levels Manager");
	m_pLevelsManager = new LevelsManager(m_pWorld, this, M2P, P2M);
	m_pLevelsManager->SetLevel(1);

	//Pause menu
	int width = Game::GetScreenWidth();
	int height = Game::GetScreenHeight();

	LogManager::Title("Pause Menu");
	m_pPauseGUI = new GUI(M2P, P2M);
	m_pPauseGUI->AddButton("CONTINUE", width / 2, static_cast<int>(height * 0.30), [this](void) {Pause(); });
	m_pPauseGUI->AddButton("MENU", width / 2, static_cast<int>(height * 0.50), [this](void) {Game::GetSceneManager()->SetNextState(SCENE_MAIN_MENU); });
	m_pPauseGUI->AddButton("EXIT", width / 2, static_cast<int>(height * 0.90), [this](void) {Game::GetInstance()->Quit(); });
	m_pPauseGUI->AddHUDElement("PAUSED!", H_STRIP_BLOCK, width / 2, 0, CENTER_TOP);

	LogManager::Title("Stats GUI");
	m_pStatsGUI = new GUI();
	m_pStatsGUI->AddHUDElement("Time: ", H_BLOCK, 10, 60, LEFT_TOP, 0.4, CENTER_LEFT);

	LogManager::Title("Win Screen");
	m_pWinScreen = new GUI();
	m_pWinScreen->AddButton("MENU", width / 2, static_cast<int>(height * 0.30), [this](void) {Game::GetSceneManager()->SetNextState(SCENE_MAIN_MENU); });
	m_pWinScreen->AddButton("EXIT", width / 2, static_cast<int>(height * 0.90), [this](void) {Game::GetInstance()->Quit(); });
	m_pWinScreen->AddHUDElement("YOU WON!", H_STRIP_BLOCK, width / 2, 0, CENTER_TOP);
}


Level::~Level()
{
	delete m_pWorld;
	m_pWorld = nullptr;

	if (m_pDebugDrawer != nullptr)
	{
		delete m_pDebugDrawer;
		m_pDebugDrawer = nullptr;
	}

	for (int i = 0; i < static_cast<signed int>(m_boxes.size()); ++i)
	{
		delete m_boxes[i];
		m_boxes[i] = nullptr;
	}

	m_boxes.clear();
	m_boxes.shrink_to_fit();

	delete m_pCar;
	m_pCar = nullptr;

	delete m_pArrow;
	m_pArrow = nullptr;

	delete m_pLevelsManager;
	m_pLevelsManager = nullptr;

	delete m_pPauseGUI;
	m_pPauseGUI = nullptr;

	delete m_pStatsGUI;
	m_pStatsGUI = nullptr;

	delete m_pWinScreen;
	m_pWinScreen = nullptr;
}

void
Level::Proccess(float deltaTime)
{
	if (!m_paused && !m_gameWon) 
	{
		m_pCar->Process(deltaTime, m_carControl, m_accelerator, m_turnAmount);
		m_pArrow->Process(deltaTime, m_pCar->m_pBody->GetPosition().x * M2P, m_pCar->m_pBody->GetPosition().y * M2P);
		m_pWorld->Step(deltaTime, 10, 8);
		m_pWorld->ClearForces();
		m_timeTaken += deltaTime;
		m_pStatsGUI->Process(deltaTime);
		m_distanceToFinish = b2Distance(m_endPoint, m_pCar->m_pBody->GetPosition());

		if (m_distanceToFinish <= 20.0f)
		{
			// Game is won
			m_gameWon = true;
			LogManager::Write("You won!");
		}
	}
	else if (m_paused)
	{
		m_pPauseGUI->Process(deltaTime);
	}
	else
	{
		m_pWinScreen->Process(deltaTime);
	}
}

void
Level::Draw(BackBuffer & backBuffer)
{
	// Update Camera Position
	m_pCam->SetPos(m_pCar->m_pBody->GetPosition());

	//Draw the level
	m_pLevelsManager->DrawLevel(backBuffer);

	for (int i = 0; i < static_cast<signed int>(m_boxes.size()); ++i)
	{
		m_boxes[i]->Draw(backBuffer);
	}

	m_pCar->Draw(backBuffer);

	if (m_debugDraw)
		m_pDebugDrawer->Draw();

	m_pArrow->Draw(backBuffer);

	if (!m_paused && !m_gameWon) {
		m_pStatsGUI->UpdateHud(0, ("Time: " + std::to_string(static_cast<int>(m_timeTaken))).c_str());
		m_pStatsGUI->Draw(backBuffer);
	}
	else if (m_paused){
		m_pPauseGUI->Draw(backBuffer);
	}
	else{
		m_pWinScreen->UpdateHud(0, ("YOU WON! It took you " + std::to_string(static_cast<int>(m_timeTaken)) + " seconds").c_str());
		m_pWinScreen->Draw(backBuffer);
	}
}

void
Level::ProcessControls()
{
	if (!m_paused && !m_gameWon) 
	{
		if (m_pInput->KeyDown(KEY_Q) || m_pInput->JoyButtonDown(JOY_SELECT))
			m_debugDraw = (m_debugDraw) ? false : true;

		if (!m_pInput->JoyControl())
		{
			if (m_pInput->KeyUp(KEY_A))
			{
				m_carControl &= ~CAR_LEFT;
				m_turnAmount = 0.0f;
			}
			if (m_pInput->KeyUp(KEY_D))
			{
				m_carControl &= ~CAR_RIGHT;
				m_turnAmount = 0.0f;
			}
			if (m_pInput->KeyUp(KEY_W))
			{
				m_carControl &= ~CAR_FORWARD;
				m_accelerator = 0.0f;
			}
			if (m_pInput->KeyUp(KEY_S))
			{
				m_carControl &= ~CAR_BACKWARD;
				m_accelerator = 0.0f;
			}

			if (m_pInput->KeyDown(KEY_A))
			{
				m_carControl |= CAR_LEFT;
				m_turnAmount = -1.0f;
			}
			if (m_pInput->KeyDown(KEY_D))
			{
				m_carControl |= CAR_RIGHT;
				m_turnAmount = 1.0f;
			}
			if (m_pInput->KeyDown(KEY_W))
			{
				m_carControl |= CAR_FORWARD;
				m_accelerator = 1.0f;
			}
			if (m_pInput->KeyDown(KEY_S))
			{
				m_carControl |= CAR_BACKWARD;
				m_accelerator = 1.0f;
			}
		}
		else
		{
			// Reset amounts to zero, ready to recieve input
			m_turnAmount = 0.0f;
			m_accelerator = 0.0f;

			if (m_pInput->LeftJoyHorizValue() < 0.0f)
			{
				m_carControl |= CAR_LEFT;
				m_turnAmount = m_pInput->LeftJoyHorizValue();
			}
			if (m_pInput->LeftJoyHorizValue() > 0.0f)
			{
				m_carControl |= CAR_RIGHT;
				m_turnAmount = m_pInput->LeftJoyHorizValue();
			}
			if (m_pInput->RightTriggerValue() > 0.0f)
			{
				m_carControl |= CAR_FORWARD;
				m_accelerator = m_pInput->RightTriggerValue();
			}
			if (m_pInput->LeftTriggerValue() > 0.0f)
			{
				m_carControl |= CAR_BACKWARD;
				m_accelerator = m_pInput->LeftTriggerValue();
			}

			if (m_pInput->LeftJoyHorizValue() == 0.0f)
				m_carControl &= ~CAR_LEFT;
			if (m_pInput->LeftJoyHorizValue() == 0.0f)
				m_carControl &= ~CAR_RIGHT;
			if (m_pInput->RightTriggerValue() < 0.1f)
				m_carControl &= ~CAR_FORWARD;
			if (m_pInput->LeftTriggerValue() < 0.1f)
				m_carControl &= ~CAR_BACKWARD;
		}
	}
	else if(m_paused)
	{
		if (m_pInput->KeyDown(KEY_W) || m_pInput->KeyDown(KEY_UP) || m_pInput->JoyHatDown(HAT_UP))
			m_pPauseGUI->IncreaseSelectedElement();
		if (m_pInput->KeyDown(KEY_S) || m_pInput->KeyDown(KEY_DOWN) || m_pInput->JoyHatDown(HAT_DOWN))
			m_pPauseGUI->DecreaseSelectedElement();
		if (m_pInput->KeyDown(KEY_RETURN) || m_pInput->KeyDown(KEY_E) || m_pInput->JoyButtonDown(JOY_A))
			m_pPauseGUI->Click();
	}
	else {
		if (m_pInput->KeyDown(KEY_W) || m_pInput->KeyDown(KEY_UP) || m_pInput->JoyHatDown(HAT_UP))
			m_pWinScreen->IncreaseSelectedElement();
		if (m_pInput->KeyDown(KEY_S) || m_pInput->KeyDown(KEY_DOWN) || m_pInput->JoyHatDown(HAT_DOWN))
			m_pWinScreen->DecreaseSelectedElement();
		if (m_pInput->KeyDown(KEY_RETURN) || m_pInput->KeyDown(KEY_E) || m_pInput->JoyButtonDown(JOY_A))
			m_pWinScreen->Click();
	}

	if (m_pInput->KeyDown(KEY_ESCAPE) || m_pInput->JoyButtonDown(JOY_START))
		Pause();

	if (m_pInput->KeyDown(KEY_GRAVE))
		LogManager::Write("");
}

void
Level::SetupWorld()
{
	// Create world and initialise debug drawer
	m_pWorld = new b2World(b2Vec2(0.0f, 0.0f));

	int centerX = Game::GetInstance()->GetScreenWidth();
	int centerY = Game::GetInstance()->GetScreenHeight();

	LogManager::Title("Creating Car");
	m_pCar = new Car(m_pWorld, 0.0f, 0.0f, 4.0f, 13.0f, Calc::DegToRadf(180.0f), M2P, P2M);
}

void
Level::Pause()
{
	m_paused = !m_paused;
}

void
Level::SetEndPoint(float x, float y)
{
	m_endPoint.Set(x, y);
	m_pArrow->SetPointTo(x * M2P, y * M2P);
}
