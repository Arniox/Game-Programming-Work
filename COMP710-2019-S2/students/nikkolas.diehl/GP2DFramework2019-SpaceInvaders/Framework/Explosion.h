#pragma once
#include "entity.h"
#include "animatedsprite.h"

class Explosion :
	public Entity
{
public:
	Explosion();
	~Explosion();

	bool Explosion::Initialise(AnimatedSprite* animatedSprite);
	void Explosion::Process(float deltaTime, bool& explode);
	void Explosion::Draw(BackBuffer& backBuffer);
	void Explosion::SetCenter(float x, float y);

	bool dead;
	int timeCount;

protected:
	AnimatedSprite* m_pAnimatedSprite;
};

