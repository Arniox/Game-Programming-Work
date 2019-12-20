#include "CarTest2.h"

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
#include "ParticleEmitter.h"

CarTest2::CarTest2()
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
, m_pEmitterTest(nullptr)
, m_paused(false)
{
	m_pBackBuffer = Game::GetBackBuffer();
	m_pInput = Game::GetInputManager();
	m_pCam = Game::GetCamera();

	M2P = Game::GetIniFile()->GetValueAsFloat("debug", "metersToPixels");
	P2M = 1.0f / M2P;

	SetupWorld();
	m_pArrow = new Arrow(0, 0);

	m_debugDraw = Game::GetIniFile()->GetValueAsBoolean("debug", "debugDrawing");

	m_pDebugDrawer = new DebugDraw(M2P, P2M);
	if (!m_pDebugDrawer->Init(m_pWorld, m_pBackBuffer))
		LogManager::Write("ERROR: Unable to init debug drawer");

	//Particles
	m_pEmitterTest = new ParticleEmitter();
	m_pEmitterTest->SetDurationOfEmission(1.0f);
	m_pEmitterTest->SetLooping(true);
	m_pEmitterTest->SetLifeTime(100.0f);
	m_pEmitterTest->SetStartSpeed();
	m_pEmitterTest->SetConeOfEmission();
	m_pEmitterTest->SetStartColor();
	m_pEmitterTest->SetEmissionRate();
	m_pEmitterTest->SetMaxParticles(200);


	//Levels
	m_pLevelsManager = new LevelsManager(m_pWorld, nullptr, M2P, P2M);
	m_pLevelsManager->SetLevel(1);

	//Pause menu
	int width = Game::GetScreenWidth();
	int height = Game::GetScreenHeight();

	m_pPauseGUI = new GUI(M2P, P2M);
	m_pPauseGUI->AddButton("CONTINUE", width / 2, static_cast<int>(height * 0.30), [this](void) {Pause(); });
	m_pPauseGUI->AddButton("MENU", width / 2, static_cast<int>(height * 0.50), [this](void) {Game::GetSceneManager()->SetNextState(SCENE_MAIN_MENU); });
	m_pPauseGUI->AddButton("EXIT", width / 2, static_cast<int>(height * 0.90), [this](void) {Game::GetInstance()->Quit(); });
	m_pPauseGUI->AddHUDElement("PAUSED!", H_STRIP_BLOCK, width / 2, 0, CENTER_TOP);
}

CarTest2::~CarTest2()
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

	delete m_pEmitterTest;
	m_pEmitterTest = nullptr;
}

void
CarTest2::Proccess(float deltaTime)
{
	if (!m_paused) 
	{
		m_pCar->Process(deltaTime, m_carControl, m_accelerator, m_turnAmount);
		m_pArrow->Process(deltaTime, m_pCar->m_pBody->GetPosition().x * P2M, m_pCar->m_pBody->GetPosition().y * P2M);
		m_pWorld->Step(deltaTime, 10, 8);
		m_pWorld->ClearForces();
		m_pEmitterTest->Process(deltaTime, 0, 0);
	}
	else 
	{
		m_pPauseGUI->Process(deltaTime);
	}
}

void
CarTest2::Draw(BackBuffer& backBuffer)
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

	m_pEmitterTest->Draw(backBuffer);

	if (m_paused)
		m_pPauseGUI->Draw(backBuffer);
}

void
CarTest2::ProcessControls()
{
	if (!m_paused)
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
	else 
	{
		if (m_pInput->KeyDown(KEY_W) || m_pInput->KeyDown(KEY_UP) || m_pInput->JoyHatDown(HAT_UP))
			m_pPauseGUI->IncreaseSelectedElement();
		if (m_pInput->KeyDown(KEY_S) || m_pInput->KeyDown(KEY_DOWN) || m_pInput->JoyHatDown(HAT_DOWN))
			m_pPauseGUI->DecreaseSelectedElement();
		if (m_pInput->KeyDown(KEY_RETURN) || m_pInput->KeyDown(KEY_E) || m_pInput->JoyButtonDown(JOY_A))
			m_pPauseGUI->Click();
	}
	
	if (m_pInput->KeyDown(KEY_ESCAPE) || m_pInput->JoyButtonDown(JOY_START))
		Pause();
}

void
CarTest2::SetupWorld()
{
	// Create world and initialise debug drawer
	m_pWorld = new b2World(b2Vec2(0.0f, 0.0f));

	int centerX = Game::GetInstance()->GetScreenWidth();
	int centerY = Game::GetInstance()->GetScreenHeight();

	m_pCar = new Car(m_pWorld, 0.0f, 0.0f, 4.0f, 13.0f, Calc::DegToRadf(180.0f), M2P, P2M);

	// Add some boxes to the world...
	m_boxes.push_back(new DebugSquare(m_pWorld, 400, 400, 4.0, M2P, P2M));
	m_boxes.push_back(new DebugSquare(m_pWorld, 150, -200, 4.0, M2P, P2M));
	m_boxes.push_back(new DebugSquare(m_pWorld, -400, 400, 4.0, M2P, P2M));
	m_boxes.push_back(new DebugSquare(m_pWorld, 0, 1000, 4.0, M2P, P2M));
	m_boxes.push_back(new DebugSquare(m_pWorld, -500, -400, 4.0, M2P, P2M));
	m_boxes.push_back(new DebugSquare(m_pWorld, -1000, 400, 4.0, M2P, P2M));
}

void 
CarTest2::Pause()
{
	m_paused = !m_paused;
}
