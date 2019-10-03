#pragma once
#include "backbuffer.h"
#include "fmod.hpp"
#include "Explosion.h"
#include <vector>

class ExplosionPool
{
public:
	ExplosionPool();
	~ExplosionPool();

	const int MAX_EXPLOSIONS = 20;

	void ExplosionPool::Initialise(BackBuffer* m_pBackBuffer);
	void ExplosionPool::Process(FMOD::System*, FMOD::Sound*, float deltaTime, bool& explode, std::vector<std::vector<int>*>& explosionCoords);

	void ExplosionPool::Draw(BackBuffer& backBuffer);

	std::vector<Explosion*> m_explosions;

	std::vector<Explosion*> ExplosionPool::GetExplosions();
	Explosion* ExplosionPool::GetEnemyAt(int input);
	Explosion* ExplosionPool::GetNewestDead();
};

