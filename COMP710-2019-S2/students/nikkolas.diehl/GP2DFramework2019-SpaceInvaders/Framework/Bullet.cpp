#include "Bullet.h"

Bullet::Bullet()
: dead(false)
{
}
Bullet::~Bullet()
{
}

void
Bullet::Process(float deltaTime) {
	Entity::Process(deltaTime);
}

void 
Bullet::SetCenter(float x, float y) {
	m_x = x;
	m_y = y;
}