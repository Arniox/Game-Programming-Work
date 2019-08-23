#include "ParticleManager.h"



ParticleManager::ParticleManager()
	:explosions(0)
	,m_gravity(0)
	,m_maxRandomAngle(0)
	,m_randomExplosionRadius(0)
	,m_maxExplosionRadius(0)
	,m_minAge(0)
	,m_maxAge(0)
	,m_maxExplosionDegredation(0)
	,m_particleCount(0)
{
}
ParticleManager::~ParticleManager()
{
	//Destroy and clean the explosion pool
	for (int i = 0; i < explosions.size(); ++i) {
		delete explosions.at(i);
		explosions.at(i) = 0;
	}
}

void
ParticleManager::Initialise(BackBuffer * backBuffer, IniParser* m_iniParser)
{
	m_gravity = m_iniParser->GetValueAsFloat("Particles", "gravity");
	m_maxRandomAngle = m_iniParser->GetValueAsInt("Particles", "max_random_angle");
	m_randomExplosionRadius = m_iniParser->GetValueAsBoolean("Particles", "random_explosion_radius");
	m_maxExplosionRadius = m_iniParser->GetValueAsInt("Particles", "max_explosion_radius");
	m_minAge = m_iniParser->GetValueAsInt("Particles", "min_age");
	m_maxAge = m_iniParser->GetValueAsInt("Particles", "max_age");
	m_maxExplosionDegredation = m_iniParser->GetValueAsFloat("Particles", "max_explosion_degredation");
	m_particleCount = m_iniParser->GetValueAsFloat("Particles", "max_particleCount");
	particleName = m_iniParser->GetValueAsString("Particles", "particle");


	m_backBuffer = backBuffer;
	//Init the particle pool
	for (int i = 0; i < MAX_PARTICLES; ++i) {
		m_explosions[i].Initialise(m_backBuffer->CreateSprite(particleName.c_str()));
	}
}

void
ParticleManager::Process(float deltaTime, bool& explode, std::vector<std::vector<int>*>& explosionCoords, int windowWidth, int windowHeight)
{
	int randomParticleCount = (rand() % m_particleCount);

	if (explode) {
		//Create new explosion for all exploion coords saved
		for (int i = 0; i < explosionCoords.size(); ++i) {
			for (int k = 0; k < randomParticleCount; ++k) {
				//In use now
				explosions.push_back(new ExplosionParticle());
				explosions.back()->Initialise(m_backBuffer->CreateSprite(particleName.c_str()));
				explosions.back()->SetCenter(explosionCoords.back()->front(), explosionCoords.back()->back(), m_randomExplosionRadius, m_maxRandomAngle, m_maxExplosionRadius, m_minAge, m_maxAge, m_maxExplosionDegredation);
				explosions.back()->dead = false;
			}
		}
	}

	//Only process living particles
	for (int t = 0; t < explosions.size(); ++t) {
		//if alive check
		if (!explosions.at(t)->dead) {
			explosions.at(t)->Process(deltaTime, windowWidth, windowHeight, m_gravity);
		} 
		else { //Kill dead
			explosions.erase(explosions.begin() + t);
		}
	}
}

void
ParticleManager::Draw(BackBuffer & backBuffer)
{
	//Only draw living explosion particles
	for (int t = 0; t < explosions.size(); ++t) {
		//if alive check
		if (!explosions.at(t)->dead) {
			explosions.at(t)->Draw(backBuffer);
		}
	}
}
