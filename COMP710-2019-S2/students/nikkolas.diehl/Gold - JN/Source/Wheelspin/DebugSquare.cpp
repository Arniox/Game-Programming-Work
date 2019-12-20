#include "DebugSquare.h"

#include "Box2D/Box2D.h"
#include "backbuffer.h"
#include "sprite.h"
#include "Calc.h"
#include "game.h"
#include "IniParser.h"

DebugSquare::DebugSquare(b2World* _pWorld, int x, int y, double scale, float _M2P, float _P2M)
: m_pBody(nullptr)
, m_pSprite(nullptr)
, pWorld(nullptr)
{
	IniParser* pIni = Game::GetIniFile();
	pWorld = _pWorld;

	M2P = _M2P;
	P2M = _P2M;

	m_pSprite = Game::GetBackBuffer()->CreateSprite("assets\\Textures\\Sprites\\default.png");
	m_pSprite->SetSpriteScaler(scale);

	b2BodyDef bodydef;
	bodydef.position.Set(x * P2M, y * P2M);
	m_pBody = pWorld->CreateBody(&bodydef);
	b2PolygonShape shape;
	shape.SetAsBox(static_cast<float32>(P2M * (m_pSprite->GetWidth() * scale) / 2), static_cast<float32>(P2M * (m_pSprite->GetHeight() * scale) / 2));
	b2FixtureDef fixturedef;
	fixturedef.shape = &shape;
	fixturedef.density = 1.0;
	//fixturedef.filter.categoryBits = 0x0008;
	//fixturedef.filter.maskBits = -1 & ~0x0004;
	m_pBody->CreateFixture(&fixturedef);
}


DebugSquare::~DebugSquare()
{
	delete m_pSprite;
	m_pSprite = nullptr;

	m_pBody = nullptr;

	pWorld = nullptr;
}

void
DebugSquare::Draw(BackBuffer & backBuffer)
{
	m_pSprite->SetX(static_cast<int>(m_pBody->GetPosition().x * M2P - ((m_pSprite->GetWidth() * m_pSprite->GetSpriteScaler()) / 2)));
	m_pSprite->SetY(static_cast<int>(m_pBody->GetPosition().y * M2P - ((m_pSprite->GetHeight() * m_pSprite->GetSpriteScaler()) / 2)));
	m_pSprite->SetAngle(Calc::RadToDeg(m_pBody->GetAngle()));
	m_pSprite->Draw(backBuffer);
}
