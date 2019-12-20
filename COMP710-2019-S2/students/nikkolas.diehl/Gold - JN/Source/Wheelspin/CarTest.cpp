#include "CarTest.h"

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

b2Vec2 CameraPos(0.0f, 0.0f);

CarTest::CarTest()
: m_carControl(0)
, m_accelerator(0.0f)
, m_turnAmount(0.0f)
, m_debugDraw(true)
, m_pDebugDrawer(nullptr)
, m_pBackBuffer(nullptr)
, m_pCam(nullptr)
, m_pCar(nullptr)
, m_pBall(nullptr)
{
	m_pBackBuffer = Game::GetBackBuffer();
	m_pInput = Game::GetInputManager();
	m_pCam = Game::GetCamera();

	M2P = Game::GetIniFile()->GetValueAsFloat("debug", "metersToPixels");
	P2M = 1.0f / M2P;

	m_pBall = Game::GetBackBuffer()->CreateSprite("assets\\Textures\\Sprites\\debug_sprite.png");
	m_pBall->SetSpriteScaler(1.0);
	m_pBall->SetX(100);
	m_pBall->SetY(100);

	SetupWorld();

	m_debugDraw = Game::GetIniFile()->GetValueAsBoolean("debug", "debugDrawing");

	if (m_debugDraw)
	{
		m_pDebugDrawer = new DebugDraw(M2P, P2M);
		if (!m_pDebugDrawer->Init(m_pWorld, m_pBackBuffer))
			LogManager::Debug("ERROR: Unable to init debug drawer");
	}

	CameraPos = m_pCar->m_pBody->GetPosition();
}


CarTest::~CarTest()
{
	delete m_pWorld;
	m_pWorld = nullptr;

	if (m_pDebugDrawer != nullptr)
	{
		delete m_pDebugDrawer;
		m_pDebugDrawer = nullptr;
	}

	if (m_pBall != nullptr)
	{
		delete m_pBall;
		m_pBall = nullptr;
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
}

void
CarTest::Proccess(float deltaTime)
{
	//wheel->Process(m_carControl);
	m_pCar->Process(deltaTime, m_carControl, m_accelerator, m_turnAmount);

	m_pWorld->Step(deltaTime, 8, 3);
}

void
CarTest::Draw(BackBuffer& backBuffer)
{
	// Update Camera Position
	m_pCam->SetPos(-m_pCar->m_pBody->GetPosition());

	m_pBall->Draw(backBuffer);

	for (int i = 0; i < static_cast<signed int>(m_boxes.size()); ++i)
	{
		m_boxes[i]->Draw(backBuffer);
	}

	m_pCar->Draw(backBuffer);

	if (m_debugDraw)
		m_pDebugDrawer->Draw();
}

void
CarTest::ProcessControls()
{
	if (m_pInput->KeyDown(KEY_Q))
		m_debugDraw = (m_debugDraw) ? false : true;

	if (m_pInput->KeyDown(KEY_ESCAPE))
		Game::GetInstance()->Quit();

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

		if (m_pInput->JoyButtonDown(JOY_SELECT))
			m_debugDraw = (m_debugDraw) ? false : true;

		if (m_pInput->JoyButtonDown(JOY_START))
			Game::GetInstance()->Quit();
	}
}

void
CarTest::SetupWorld()
{
	// Create world and initialise debug drawer
	m_pWorld = new b2World(b2Vec2(0.0f, 0.0f));

	int centerX = Game::GetInstance()->GetScreenWidth();
	int centerY = Game::GetInstance()->GetScreenHeight();

	m_pCar = new Car(m_pWorld, 0.0f, 0.0f, 2.0f, 4.0f, 180.0f, M2P, P2M);
	/*
	b2BodyDef bodydef;
	bodydef.position.Set(400 * P2M, 400 * P2M);
	b2Body* body = m_pWorld->CreateBody(&bodydef);
	b2PolygonShape shape;
	shape.SetAsBox(P2M * 80 / 2, P2M * 80 / 2);
	b2FixtureDef fixturedef;
	fixturedef.shape = &shape;
	fixturedef.density = 1.0;
	//fixturedef.filter.categoryBits = 0x0008;
	body->CreateFixture(&fixturedef);
	*/
	// Add some boxes to the world...
	m_boxes.push_back(new DebugSquare(m_pWorld, 400, 400, 4.0, M2P, P2M));
	m_boxes.push_back(new DebugSquare(m_pWorld, 150, -200, 4.0, M2P, P2M));
	m_boxes.push_back(new DebugSquare(m_pWorld, -400, 400, 4.0, M2P, P2M));
	m_boxes.push_back(new DebugSquare(m_pWorld, 0, 1000, 4.0, M2P, P2M));
	m_boxes.push_back(new DebugSquare(m_pWorld, -500, -400, 4.0, M2P, P2M));
	m_boxes.push_back(new DebugSquare(m_pWorld, -1000, 400, 4.0, M2P, P2M));
	m_boxes.push_back(new DebugSquare(m_pWorld, 250, 250, 1.0, M2P, P2M));
}