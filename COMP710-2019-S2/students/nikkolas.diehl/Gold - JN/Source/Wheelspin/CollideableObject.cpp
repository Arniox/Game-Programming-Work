#include "CollideableObject.h"

#include "Box2D/Box2D.h"
#include "backbuffer.h"
#include "sprite.h"
#include "Calc.h"
#include "game.h"
#include "IniParser.h"

CollideableObject::CollideableObject(b2World* pWorld, int x, int y, double scale)
: m_pBody(nullptr)
, m_pSprite(nullptr)
{
}

CollideableObject::CollideableObject()
{
}


CollideableObject::~CollideableObject()
{
}

void
CollideableObject::Draw(BackBuffer & backBuffer)
{
	m_pSprite->SetX(static_cast<int>(m_pBody->GetPosition().x * M2P - ((m_pSprite->GetWidth() * m_pSprite->GetSpriteScaler()) / 2)));
	m_pSprite->SetY(static_cast<int>(m_pBody->GetPosition().y * M2P - ((m_pSprite->GetHeight() * m_pSprite->GetSpriteScaler()) / 2)));
	m_pSprite->SetAngle(Calc::RadToDeg(m_pBody->GetAngle()));
	m_pSprite->Draw(backBuffer);
}
