#pragma once
#include <vector>
#include "Bullet.h"
#include "backbuffer.h"
#include "fmod.hpp"

class BulletPool
{
public:
	BulletPool();
	~BulletPool();

	//Bullet vector
	std::vector<Bullet*> m_pBullets;
	std::vector<Bullet*>::iterator bulletIterator;

	void BulletPool::Initialise(BackBuffer* m_pBackBuffer);

	void BulletPool::Process(FMOD::System* m_fmodSystem, FMOD::Sound* sFireSound, float deltaTime, bool& shoot, float posX, float posY);
	void BulletPool::Draw(BackBuffer& backBuffer);

	std::vector<Bullet*> BulletPool::GetBullets();
	Bullet* BulletPool::GetBulletAt(int input);

	Bullet* BulletPool::GetNewestDead();

protected:
	const int MAX_BULLETS = 20;
};

