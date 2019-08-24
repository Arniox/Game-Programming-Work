#pragma once
#include "backbuffer.h"
#include "ExplosionParticle.h"
#include "iniParser.h"
#include <vector>

class ParticleManager
{
public:
	ParticleManager();
	~ParticleManager();

	void ParticleManager::Initialise(BackBuffer* backBuffer, IniParser* m_iniParser);
	void ParticleManager::Process(float deltaTime, bool& explode, std::vector<std::vector<int>*>& explosionCoords, int windowWidth, int windowHeight);
	void ParticleManager::Draw(BackBuffer& backBuffer);
	int ParticleManager::GetParticleCount();

	BackBuffer* m_backBuffer;

	//Explosions
	const int MAX_PARTICLES = 100000;
	ExplosionParticle m_explosions[100000];
	ExplosionParticle m_OutPut[1000];
	std::vector<ExplosionParticle*> explosions;

	//Get newest dead
	ExplosionParticle* ParticleManager::GetNewestDead();

	//Particle variables
	string particleName;
	float m_gravity;
	int m_maxRandomAngle;
	bool m_randomExplosionRadius;
	int m_maxExplosionRadius;
	int m_minAge;
	int m_maxAge;
	float m_maxExplosionDegredation;
	int m_particleCount;
};

