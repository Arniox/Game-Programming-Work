#include "Test2.h"

#include "backbuffer.h"
#include "InputManager.h"
#include "game.h"
#include "DebugDraw.h"
#include "logmanager.h"
#include "InputManager.h"

Test2::Test2()
: m_pDebugDrawer(nullptr)
, m_pBackBuffer(nullptr)
, m_pInput(nullptr)
, m_debug(false)
, m_clearWorld(false)
{
	m_pBackBuffer = Game::GetBackBuffer();
	m_width = Game::GetInstance()->GetScreenWidth();
	m_height = Game::GetInstance()->GetScreenHeight();

	m_pInput = Game::GetInputManager();

	m_debug = true;

	int m_centerX = Game::GetInstance()->GetScreenWidth() / 2;
	
	// Create world and initialise debug drawer
	m_pWorld = new b2World(b2Vec2(0.0f, 9.81f));

	m_pDebugDrawer = new DebugDraw(M2P, P2M);
	if (!m_pDebugDrawer->Init(m_pWorld, m_pBackBuffer))
		LogManager::Debug("ERROR: Unable to ini debug drawer");

	SetupWorld();
}

Test2::~Test2()
{
	delete m_pWorld;
	m_pWorld = nullptr;

	if (m_pDebugDrawer != nullptr)
	{
		delete m_pDebugDrawer;
		m_pDebugDrawer = nullptr;
	}
	
}

void
Test2::Proccess(float deltaTime)
{
	//world->Step(deltaTime, 8, 3);
	//world->Step(1.0 / 60.0, 8, 3);

	if (m_clearWorld)
		ResetWorld();

	m_pWorld->Step(1.0f / 30.0f, 8, 3);
}

void
Test2::Draw(BackBuffer& backBuffer)
{
	if (m_debug && !m_clearWorld)
		m_pDebugDrawer->Draw();
}

void
Test2::ProcessControls()
{
	if (m_pInput->MousePressed(MB1))
		AddRect(m_pInput->m_mouseX, m_pInput->m_mouseY, 20, 20, true);

	if (m_pInput->MousePressed(MB2))
		AddBall(m_pInput->m_mouseX, m_pInput->m_mouseY, 0.5f, 0.8f);

	if (m_pInput->KeyDown(KEY_R))
		m_clearWorld = true;
	
	if (m_pInput->KeyDown(KEY_ESCAPE))
		Game::GetInstance()->Quit();
}

void
Test2::ResetWorld()
{
	LogManager::Debug("Clearing world...");
	
	if (m_pWorld != nullptr)
	{
		delete m_pWorld;
		m_pWorld = nullptr;
	}

	m_pWorld = new b2World(b2Vec2(0.0f, 9.81f));
	m_clearWorld = false;

	SetupWorld();
	LogManager::Debug("World Reset");
}

void
Test2::SetupWorld()
{

	/*
	// Add ground
	AddRect(m_width / 2, m_height, m_width, 30, false);
	// Add Walls
	AddRect(0, m_height / 2, 30, m_height, false); // Left Wall
	AddRect(m_width, m_height / 2, 30, m_height, false); // Right Wall

	// Create ball 1
	b2BodyDef bd;
	bd.type = b2_dynamicBody;
	bd.position.Set(static_cast<float>(150 * P2M), 0.5f);
	b2Body* body = m_pWorld->CreateBody(&bd);
	b2CircleShape shape;
	shape.m_radius = 0.5f;
	b2FixtureDef fd;
	fd.shape = &shape;
	fd.density = 1.0f;
	fd.restitution = 0.9f;
	body->CreateFixture(&fd);

	// Create ball2
	bd.position.Set(static_cast<float>(150 * P2M), 6.0f);
	body = m_pWorld->CreateBody(&bd);
	shape.m_radius = 2.5f;
	fd.restitution = 0.0f;
	body->CreateFixture(&fd);

	//
	*/
	SliderCrank();
}

void
Test2::AddRect(int x, int y, int w, int h, bool dyn = true)
{
	b2BodyDef bodydef;
	bodydef.position.Set(x * P2M, y * P2M);
	if (dyn)
		bodydef.type = b2_dynamicBody;

	b2Body* body = m_pWorld->CreateBody(&bodydef);

	b2PolygonShape shape;
	shape.SetAsBox(P2M * w / 2, P2M * h / 2);

	b2FixtureDef fixturedef;
	fixturedef.shape = &shape;
	fixturedef.density = 1.0;
	body->CreateFixture(&fixturedef);
}

void
Test2::AddBall(int x, int y, float radius, float restitution)
{
	b2BodyDef bd;
	bd.type = b2_dynamicBody;
	bd.position.Set(static_cast<float>(x * P2M), static_cast<float>(y * P2M));
	b2Body* body = m_pWorld->CreateBody(&bd);
	b2CircleShape shape;
	shape.m_radius = radius;
	b2FixtureDef fd;
	fd.shape = &shape;
	fd.density = 1.0f;
	fd.restitution = restitution;
	body->CreateFixture(&fd);
}