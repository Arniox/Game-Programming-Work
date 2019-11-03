#include "Camera.h"
#include "IniParser.h"
#include "game.h"

Camera::Camera(float _M2P, float _P2M)
: x(0)
, y(0)
, m_maxX(0)
, m_maxY(0)
, m_minX(0)
, m_minY(0)
, m_paddingX(0)
, m_paddingY(0)
, m_screenCenterX(0)
, m_screenCenterY(0)
{
	M2P = _M2P;
	P2M = _P2M;

	m_paddingX = Game::GetIniFile()->GetValueAsInt("camera", "xPadding");
	m_paddingY = Game::GetIniFile()->GetValueAsInt("camera", "yPadding");

	m_screenCenterX = Game::GetInstance()->GetScreenWidth() / 2;
	m_screenCenterY = Game::GetInstance()->GetScreenHeight() / 2;
}


Camera::~Camera()
{
}

void
Camera::SetPos(int x, int y)
{
	this->x = x;
	this->y = y;
	Update();
}

void
Camera::SetPos(b2Vec2 pos)
{
	this->x = static_cast<int>(-pos.x * M2P) + m_screenCenterX;
	this->y = static_cast<int>(-pos.y * M2P) + m_screenCenterY;
	Update();
}

void
Camera::Update()
{
	m_minX = -x - m_paddingX;
	m_maxX = -x + m_paddingX + (m_screenCenterX * 2);
	m_minY = -y - m_paddingY;
	m_maxY = -y + m_paddingY + (m_screenCenterY * 2);
}

bool
Camera::InBounds(int xPos, int yPos)
{
	if (xPos >= m_minX && xPos<= m_maxX)
	{
		if (yPos >= m_minY && yPos <= m_maxY)
		{
			return true;
		}
	}
	return false;
}
