#include "ExplosionPool.h"



ExplosionPool::ExplosionPool()
	:m_explosions(0)
{
}
ExplosionPool::~ExplosionPool()
{
	//Destroy and clean the explosion pool
	for (int i = 0; i < m_explosions.size(); ++i) {
		delete m_explosions.at(i);
		m_explosions.at(i) = 0;
		
	}
}

void 
ExplosionPool::Initialise(BackBuffer * m_pBackBuffer)
{
	//Initialise the explosion pool with a defined amount of explosions
	for (int i = 0; i < MAX_EXPLOSIONS; ++i) {
		m_explosions.push_back(new Explosion());
		m_explosions.at(i)->Initialise(m_pBackBuffer->CreateAnimatedSprite("assets/Sprites/explosionGif.png", 5, 320/5));
	}
}

void 
ExplosionPool::Process(FMOD::System* m_fmodSystem, FMOD::Sound* sExplosionSound, float deltaTime, bool& explode, std::vector<std::vector<int>*>& explosionCoords)
{
	if (explode) {
		//Create a new explosion for all explosion coords saved
		for (int i = 0; i < explosionCoords.size(); ++i) {
			Explosion* explosion = GetNewestDead();
			if (explosion != 0) {
				//In use now
				explosion->dead = false;
				explosion->SetCenter(explosionCoords.back()->front(), explosionCoords.back()->back());
				//Play explosion
				m_fmodSystem->playSound(sExplosionSound, 0, false, 0);

				//remove explosion coord since it's finished
				explosionCoords.pop_back();
			}
			//Else no other explosions are available
		}
	}

	//Only process living explosions
	for (int k = 0; k < m_explosions.size(); ++k) {
		//if alive check
		if (!m_explosions.at(k)->dead) {
			m_explosions.at(k)->Process(deltaTime, explode);
		}
	}
}

void 
ExplosionPool::Draw(BackBuffer & backBuffer)
{
	//Draw each explosion if it is alive
	for (int i = 0; i < m_explosions.size(); ++i) {
		if (!m_explosions.at(i)->dead) {
			m_explosions.at(i)->Draw(backBuffer);
		}
	}
}

std::vector<Explosion*> 
ExplosionPool::GetExplosions()
{
	return m_explosions;
}

Explosion * 
ExplosionPool::GetEnemyAt(int input)
{
	return m_explosions.at(input);
}

Explosion * 
ExplosionPool::GetNewestDead()
{
	//Return the last dead (unsed) explosion
	for (int i = 0; i < m_explosions.size(); ++i) {
		if (m_explosions.at(i)->dead) {
			return m_explosions.at(i);
		}
	}

	//If found nothing, then return null
	return 0;
}
