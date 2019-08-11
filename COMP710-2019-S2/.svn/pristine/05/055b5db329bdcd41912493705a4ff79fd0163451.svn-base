#include "Explosion.h"
#include <cassert>

Explosion::Explosion()
:dead(true)
,timeCount(0)
{
}
Explosion::~Explosion()
{
}

bool Explosion::Initialise(AnimatedSprite * animatedSprite)
{
	assert(animatedSprite);
	m_pAnimatedSprite = animatedSprite;

	return (true);
}

void Explosion::Process(float deltaTime, bool& explode)
{
	if (!dead) {
		timeCount++;
		if (timeCount >= 60) {
			dead = true;
			timeCount = 0;
			explode = false;
		}

		m_pAnimatedSprite->Process(deltaTime);
	}
}

void Explosion::Draw(BackBuffer & backBuffer)
{
	assert(m_pAnimatedSprite);
	m_pAnimatedSprite->SetX(static_cast<int>(m_x));
	m_pAnimatedSprite->SetY(static_cast<int>(m_y));
	m_pAnimatedSprite->Draw(backBuffer);
}

void Explosion::SetCenter(float x, float y)
{
	m_x = x;
	m_y = y;
}
