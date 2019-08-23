#pragma once
#include "backbuffer.h"
#include <vector>
#include "entity.h"
#include <math.h>

//Definitions
#define degreesToRadians(angleDegrees) (angleDegrees * M_PI / 180.0)
#define radiansToDegrees(angleRadians) (angleRadians * 180.0 / M_PI)

class ExplosionParticle :
	public Entity
{
public:
	ExplosionParticle();
	~ExplosionParticle();

	void ExplosionParticle::SetCenter(float x, float y, bool randomRadius, int max_angle, int max_explosion_radius, int min_age, int maximum_age, float max_explosion_degredation);
	void ExplosionParticle::Process(float deltaTime, int windowWidth, int windowHeight, float gravity);
	float ExplosionParticle::randMtoN(float M, float N);

	//Main Variables
	bool dead;
	float age;
	float max_age;

	//Movement
	float explosionRadius;
	float randomAngle;
	float randomExplosionDegredation;
	float R;
	float dy;
	float dx;
	float vx;
	float vy;
	float gravity;
};

