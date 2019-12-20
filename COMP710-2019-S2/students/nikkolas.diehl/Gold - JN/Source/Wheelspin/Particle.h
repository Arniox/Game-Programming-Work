#ifndef __PARTICLE_H__
#define __PARTICLE_H__

//Libraries
#include <SDL.h>

//Locals
#include "Calc.h"

//Foward Declarations
class BackBuffer;
class Vector2;

class Particle
{
public:
	Particle();
	~Particle();

	void PreCreate(SDL_Color* color, float lifeTime, float _xVelocity, float _yVelocity, int particleSize);

	void Process(double deltaTime);
	void Draw(BackBuffer& backBuffer);

	bool IsDead();
	void Kill();
	void Spawn(int X, int Y, float xVel, float yVel);

private:
	Vector2* m_pPosition;
	SDL_Color* m_pStartColor;

	bool m_dead;

	float m_lifeTime;
	float m_xVelocity;
	float m_yVelocity;
	float m_lifeCounter;
	int m_size;
};

#endif

