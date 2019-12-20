//Locals
#include "Particle.h"
#include "backbuffer.h"
#include "Vector2.h"
#include "game.h"

Particle::Particle()
: m_pPosition()
, m_pStartColor(nullptr)
, m_dead(true)
, m_lifeTime(0.0f)
, m_lifeCounter(0.0f)
, m_xVelocity(0.0f)
, m_yVelocity(0.0f)
, m_size(0)
{
}
Particle::~Particle()
{
	delete m_pPosition;
	m_pPosition = nullptr;
}

void 
Particle::PreCreate(SDL_Color* color, float lifeTime, float _xVelocity, float _yVelocity, int particleSize)
{
	m_pPosition = new Vector2(0, 0);
	m_dead = true;
	m_pStartColor = color;

	m_lifeTime = lifeTime;
	m_xVelocity = _xVelocity;
	m_yVelocity = _yVelocity;
	m_size = particleSize;

	m_lifeCounter = m_lifeTime;
}

void 
Particle::Process(double deltaTime)
{
	//Kill old particles
	if (!m_dead) {
		m_lifeCounter -= static_cast<float>(deltaTime);
		if (m_lifeCounter <= 0) {
			m_dead = true;
			m_lifeCounter = m_lifeTime;

			m_pPosition->x = 0;
			m_pPosition->y = 0;
		}
	}

	m_pPosition->x += m_xVelocity;
	m_pPosition->y += m_yVelocity;

}

void 
Particle::Draw(BackBuffer& backBuffer)
{
	if (!m_dead) 
	{
		backBuffer.SetDrawColour(m_pStartColor->r, m_pStartColor->g, m_pStartColor->b, m_pStartColor->a);
		backBuffer.DrawRectangle(
			static_cast<int>(m_pPosition->x + m_size / 2.0),
			static_cast<int>(m_pPosition->y + m_size / 2.0),
			static_cast<int>(m_pPosition->x - m_size / 2.0),
			static_cast<int>(m_pPosition->y - m_size / 2.0)
		);
	}
}

bool 
Particle::IsDead()
{
	return m_dead;
}

void 
Particle::Kill()
{
	m_dead = true;
}

void 
Particle::Spawn(int X, int Y, float xVel, float yVel)
{
	m_pPosition->x = X;
	m_pPosition->y = Y;

	m_xVelocity = xVel;
	m_yVelocity = yVel;

	m_dead = false;
}
