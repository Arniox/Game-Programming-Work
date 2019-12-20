#include "Arrow.h"

#include "backbuffer.h"
#include"game.h"
#include "Calc.h"
#include "sprite.h"

#include "logmanager.h"
#include <string>
Arrow::Arrow(int x, int y)
: m_pSprite(nullptr)
, m_x(0)
, m_y(0)
, m_angle(0.0f)
{
	m_pSprite = Game::GetBackBuffer()->CreateSprite("assets\\Textures\\Sprites\\arrow.png");
	m_x = x;
	m_y = y;

	m_pSprite->SetX(0);
	m_pSprite->SetY(Game::GetInstance()->GetScreenHeight() - 128);
}

Arrow::~Arrow()
{
	delete m_pSprite;
	m_pSprite = nullptr;
}

void
Arrow::Process(float deltaTime, float x, float y)
{
	m_angle = atan2f(m_y - y, m_x - x);
}

void
Arrow::Draw(BackBuffer& backBuffer)
{
	m_pSprite->SetAngle(Calc::RadToDegf(m_angle));
	m_pSprite->Draw(backBuffer, 0xFF, true);
}

void
Arrow::SetPointTo(float x, float y)
{
	m_x = static_cast<int>(x) + 64;
	m_y = static_cast<int>(y) + 64;
}
