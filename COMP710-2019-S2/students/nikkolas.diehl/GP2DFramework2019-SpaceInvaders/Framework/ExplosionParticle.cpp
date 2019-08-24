#include "ExplosionParticle.h"
#include "logmanager.h"
#include <string>
#include <random>
#include <time.h>

ExplosionParticle::ExplosionParticle()
:dead(true)
,age(0)
,max_age(0)
,explosionRadius(0)
,randomAngle(0)
,R(0)
,dy(0)
,dx(0)
,vx(0)
,vy(0)
,gravity(0.0f)
,movementMultiplier(0)
{
	gravity = 3;
}
ExplosionParticle::~ExplosionParticle()
{
}

void 
ExplosionParticle::SetCenter(float x, float y, bool randomRadius, int max_angle, int max_explosion_radius, int min_age, int maximum_age, float max_explosion_degredation)
{
	randomAngle = randMtoN(0, max_angle);
	explosionRadius = (randomRadius ? randMtoN(0, max_explosion_radius) : max_explosion_radius);
	max_age = randMtoN(min_age, maximum_age);
	randomExplosionDegredation = randMtoN(0, max_explosion_degredation);

	movementMultiplier = 1;

	R = degreesToRadians(randomAngle);
	dy = explosionRadius * cosf(R);
	dx = explosionRadius * sinf(R);
	
	float shift = 10;

	m_x = x + shift;
	m_y = y + shift;
}


void
ExplosionParticle::Process(float deltaTime, int windowWidth, int windowHeight, float gravity)
{
	age += deltaTime;
	//Kill if old
	if (age >= max_age) {
		dead = true;
	}

	//If bounce from floor (reduce speed by value%)
	if (m_x >= windowWidth - 5) {
		m_x = windowWidth - 5;
		vx *= -randomExplosionDegredation;
	}
	else if (m_x <= 5) {
		m_x = 5;
		vx *= -randomExplosionDegredation;
	}
	//If bounce from wall (reduce speed by value%)
	if (m_y >= windowHeight - 5 || m_y <= 5) {
		m_y = windowHeight-6;
		vx *= randomExplosionDegredation;
		vy *= -randomExplosionDegredation;
		movementMultiplier = 0;
	}
	else {
		movementMultiplier = 1;
	}

	//Move
	dy += gravity;
	vx += dx * deltaTime * 2;
	vy += dy * deltaTime * 2;
	m_x += vx * deltaTime * 2;
	m_y += vy * (deltaTime * movementMultiplier) * 2;

}

float 
ExplosionParticle::randMtoN(float M, float N)
{
	return M + (rand() / (RAND_MAX / (N - M)));
}
