#include "Test.h"

#include "sprite.h"
#include "backbuffer.h"
#include "game.h"
#include "DebugDraw.h"
#include "logmanager.h"
#include "InputManager.h"
#include "Calc.h"

#include "Wheel.h"

#define PTM_RATIO 32.0
#define VELOCITY_ITERATIONS 8
#define POSITION_ITERATIONS 3

Test::Test()
: m_pWorld(nullptr)
, m_pDebugDrawer(nullptr)
, delayCount(0)
{
	m_centerX = Game::GetInstance()->GetScreenWidth() / 2;
	m_centerY = Game::GetInstance()->GetScreenHeight() / 2;
	left = Game::GetBackBuffer()->CreateSprite("assets\\Textures\\Sprites\\debug_sprite.png");
	right = Game::GetBackBuffer()->CreateSprite("assets\\Textures\\Sprites\\debug_sprite.png");
	lt = Game::GetBackBuffer()->CreateSprite("assets\\Textures\\Sprites\\debug_sprite.png");
	rt = Game::GetBackBuffer()->CreateSprite("assets\\Textures\\Sprites\\debug_sprite.png");

	lt->SetX(m_centerX - 150);
	lt->SetY(m_centerY);

	rt->SetX(m_centerX + 150);
	rt->SetY(m_centerY);

	left->SetX(m_centerX - 50);
	left->SetY(m_centerY);

	right->SetX(m_centerX + 50);
	right->SetY(m_centerY);

	m_pInput = Game::GetInputManager();

	b2Vec2 gravity(0.0f, 9.0f);
	m_pWorld = new b2World(gravity);
	addRect(Game::GetInstance()->GetScreenWidth() / 2, Game::GetInstance()->GetScreenHeight() - 50, Game::GetInstance()->GetScreenWidth() - 10, 30, false);

	
	m_pBall = Game::GetBackBuffer()->CreateSprite("assets\\Textures\\Sprites\\debug_sprite.png");
	m_pBall->SetSpriteScaler(1.0);

	/*
	b2BodyDef balldef;
	balldef.type = b2_dynamicBody;
	balldef.position.Set(static_cast<float>(m_centerX) * P2M, 15.0f * P2M);
	b2Body* body = m_pWorld->CreateBody(&balldef);
	b2CircleShape shape;
	shape.m_radius = (ball->GetWidth() / 2) * P2M;
	Body->CreateFixture(&shape, 1.0f);
	*/
	b2BodyDef bd;
	bd.type = b2_dynamicBody;
	bd.position.Set(static_cast<float>(m_centerX) * P2M, 15.0f * P2M);
	Body = m_pWorld->CreateBody(&bd);
	b2CircleShape shape;
	shape.m_radius = m_pBall->GetWidth() / 2 * P2M;
	Body->CreateFixture(&shape, 10.0f);

	//b2FixtureDef ballfix;
	//ballfix.shape = &shape;
	//ballfix.density = 1.0f;
	//Body->CreateFixture(&ballfix);

	m_pDebugDrawer = new DebugDraw(M2P, P2M);
	if (!m_pDebugDrawer->Init(m_pWorld, Game::GetBackBuffer()))
		LogManager::Debug("ERROR: Unable to ini debug drawer");
}


Test::~Test()
{
	delete m_pWorld;
	m_pWorld = nullptr;

	delete m_pBall;
	m_pBall = nullptr;
}

void
Test::Proccess(float deltaTime)
{
	m_pWorld->Step(1.0f / 60.0f, VELOCITY_ITERATIONS, POSITION_ITERATIONS);
	//m_pWorld->Step(deltaTime, VELOCITY_ITERATIONS, POSITION_ITERATIONS);
}

void
Test::Draw(BackBuffer& backBuffer)
{
	left->Draw(backBuffer);
	right->Draw(backBuffer);
	lt->Draw(backBuffer);
	rt->Draw(backBuffer);
	
	m_pBall->SetX(static_cast<int>(-(m_pBall->GetWidth() / 2) + Body->GetPosition().x * M2P));
	m_pBall->SetY(static_cast<int>(-(m_pBall->GetHeight() / 2) + Body->GetPosition().y * M2P));
	m_pBall->SetAngle(Calc::RadToDeg(Body->GetAngle()));
	m_pBall->Draw(backBuffer);

	m_pDebugDrawer->Draw();
}

void
Test::ProcessControls()
{
	left->SetX(static_cast<int>(-50 + m_centerX + (m_pInput->LeftJoyHorizValue() * 50)));
	left->SetY(static_cast<int>(m_centerY + (m_pInput->LeftJoyVertValue() * 50)));

	right->SetX(static_cast<int>(50 + m_centerX + (m_pInput->RightJoyHorizValue() * 50)));
	right->SetY(static_cast<int>(m_centerY + (m_pInput->RightJoyVertValue() * 50)));

	lt->SetY(static_cast<int>(m_centerY + (m_pInput->LeftTriggerValue() * -50)));
	rt->SetY(static_cast<int>(m_centerY + (m_pInput->RightTriggerValue() * -50)));

	if (m_pInput->MousePressed(MB2)) {
		addRect(m_pInput->m_mouseX, m_pInput->m_mouseY, 20, 20, true);
	}

	if (m_pInput->KeyDown(KEY_ESCAPE))
		Game::GetInstance()->Quit();
}

void
Test::addRect(int x, int y, int w, int h, bool dyn = true)
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
