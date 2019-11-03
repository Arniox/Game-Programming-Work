
//Locals
#include "ParticleEmitter.h"
#include "backbuffer.h"
#include "Particle.h"
#include "game.h"
#include "logmanager.h"

ParticleEmitter::ParticleEmitter()
: m_vParticles()
, m_durationOfEmission(0.0f)
, m_durationCount(0.0f)
, m_lifeTime(0.0f)
, m_velocity(0.0f)
, m_coneOfEmission(0.0f)
, m_angle(0.0f)
, m_maxParticles(0)
, m_emissionRate(0)
, emitterX(0)
, emitterY(0)
, m_variableFlags()
, EMIITTING(false)
, m_looping(false)
, m_particleSize(0)
{
	m_variableFlags = new VariableFlags();
}
ParticleEmitter::~ParticleEmitter()
{
	for (int i = 0; i < static_cast<signed int>(m_vParticles.size()); ++i) {
		delete m_vParticles[i];
		m_vParticles[i] = nullptr;
	}
	m_vParticles.clear();
	m_vParticles.shrink_to_fit();

	delete m_variableFlags;
	m_variableFlags = nullptr;
}

//If no value is given, a random value between 1 and 10 is set
void 
ParticleEmitter::SetDurationOfEmission()
{
	SetDurationOfEmission(static_cast<float>(Calc::Random(1.0, 10.0)));
}

//How long do particles continue to emit after emission is called
//Duration in seconds as floating point number
void 
ParticleEmitter::SetDurationOfEmission(float in)
{
	m_durationOfEmission = in;
	m_durationCount = m_durationOfEmission;
	m_variableFlags->durOfEmSet = true;
}

//Set looping by default to false
void 
ParticleEmitter::SetLooping()
{
	SetLooping(false);
}

void 
ParticleEmitter::SetLooping(bool in)
{
	m_looping = in;
	m_variableFlags->loopSet = true;
}

//If no value is given, then a random value between 1 and 10 is set
void
ParticleEmitter::SetLifeTime()
{
	SetLifeTime(static_cast<float>(Calc::Random(1.0, 10.0)));
}

//The life time of all particles in seconds as a floating point number
void 
ParticleEmitter::SetLifeTime(float in)
{
	m_lifeTime = in;
	m_variableFlags->lifeTimeSet = true;
}

//If no value is set, then a random value between 1 and 10
void 
ParticleEmitter::SetStartSpeed()
{
	SetStartSpeed(
		static_cast<float>(Calc::Random(-10.0, 10.0))
	);
}

//The velocity of a particle when spawned
void 
ParticleEmitter::SetStartSpeed(float in)
{
	m_velocity = in;
	m_variableFlags->startSpeedSet = true;
}

//If no calue is set, then set PI * 2
void 
ParticleEmitter::SetConeOfEmission()
{
	SetConeOfEmission(Calc::RadToDegf(TAU), 0.0f);
}

//The cone of emission as floating point degrees and cone size in degrees
void
ParticleEmitter::SetConeOfEmission(float coneSize, float angle)
{
	m_coneOfEmission = coneSize;
	m_halfCone = m_coneOfEmission / 2;
	m_angle = angle;

	m_anglePlusCone = Calc::DegToRadf(m_angle + m_halfCone);
	m_angleMinusCone = Calc::DegToRadf(m_angle - m_halfCone);

	m_variableFlags->coneOfEmSet = true;
}

//If no value is set then set a random color
void 
ParticleEmitter::SetStartColor()
{
	m_pStartColor.r = static_cast<unsigned char>(rand() % 256);
	m_pStartColor.g = static_cast<unsigned char>(rand() % 256);
	m_pStartColor.b = static_cast<unsigned char>(rand() % 256);
	m_pStartColor.a = 255;
	m_variableFlags->colSet = true;
}

//Set the start color of a particle 
void 
ParticleEmitter::SetStartColor(SDL_Color& in)
{
	m_pStartColor = in;
	m_variableFlags->colSet = true;
}

//If no value is set then a random value between 1 and 5
void 
ParticleEmitter::SetEmissionRate()
{
	SetEmissionRate(static_cast<int>(Calc::Random(1.0, 5.0)));
}

//The rate that particles spawn. When Emit is called, a loop based on the emission rate will be called
void
ParticleEmitter::SetEmissionRate(int in)
{
	m_emissionRate = in;
	m_variableFlags->emRateSet = true;
}

//If value not given, then a random value between 5 and 10
void 
ParticleEmitter::SetParticleSize()
{
	SetParticleSize(static_cast<int>(Calc::Random(2.0, 10.0)));
}

void 
ParticleEmitter::SetParticleSize(bool random)
{
	SetParticleSize(-999);
}

void 
ParticleEmitter::SetParticleSize(int in)
{
	m_particleSize = in;
	m_variableFlags->sizeSet = true;
}

//If no value is set then a random value between 10 and 100
void 
ParticleEmitter::SetMaxParticles()
{
	SetMaxParticles(static_cast<int>(Calc::Random(10.0, 100.0)));
}

//The maximum number of particles for a given emitter as an int
void 
ParticleEmitter::SetMaxParticles(int in)
{
	m_maxParticles = in;
	if (m_variableFlags->CheckAll()) {
		PreWarmArray(in);
		LogManager::Debug(
			(
				"SUCCESS - Particle Emitter has been created. Duration of emission: " +
				std::to_string(m_durationOfEmission) +
				", Life Time: " + std::to_string(m_lifeTime) +
				", Start Speed: " + std::to_string(m_velocity) +
				", Cone of Emission: " + std::to_string(m_coneOfEmission) +
				", Emission Rate: " + std::to_string(m_emissionRate) +
				", Particle Size: " + std::to_string(m_particleSize) + 
				", Max Particles: " + std::to_string(m_maxParticles)
			).c_str());
	}
	else {
		LogManager::Debug("ERROR - All Variables MUST be set prior to particles being created. Even if just 0.0");
		Game::GetInstance()->Quit();
	}
}

float 
ParticleEmitter::GetConeSize()
{
	return m_coneOfEmission;
}

void 
ParticleEmitter::Emission()
{
	EMIITTING = true;
}

//Process all living particles for this emitter
void 
ParticleEmitter::Process(double deltaTime, int x, int y)
{
	for (int i = 0; i < static_cast<signed int>(m_vParticles.size()); ++i) 
	{
		m_vParticles[i]->Process(deltaTime);
	}

	emitterX = x;
	emitterY = y;

	//If it's looping, keep calling emission when the particle emitter has finished emitting
	if (m_looping) 
	{
		if (!EMIITTING) 
		{
			Emission();
		}
	}

	//If emitting, then count down duration count and emit particles
	if (EMIITTING) {
		if (m_durationCount > 0) {
			Emit(x, y);
			m_durationCount -= static_cast<float>(deltaTime);
		}
		else {
			//Stop emission
			m_durationCount = m_durationOfEmission;
			EMIITTING = false;
		}
	}
}

//Draw all living particles for this emitter
void 
ParticleEmitter::Draw(BackBuffer & backBuffer)
{
	for (int i = 0; i < static_cast<signed int>(m_vParticles.size()); ++i)
	{
		m_vParticles[i]->Draw(backBuffer);
	}
}

//Emit new particles (spawn new particles)
void 
ParticleEmitter::Emit(int X, int Y)
{
	for (int i = 0; i < m_emissionRate; ++i) {
		Particle* pParticleToSpawn = FindNewestDead();
		if (pParticleToSpawn != nullptr) {

			pParticleToSpawn->Spawn(
				X, Y,
				m_velocity * cosf(static_cast<float>(Calc::Random(m_angleMinusCone, m_anglePlusCone))),
				m_velocity * sinf(static_cast<float>(Calc::Random(m_angleMinusCone, m_anglePlusCone)))
			);
		}
	}
}

//Find the closest dead particle in the array
Particle* 
ParticleEmitter::FindNewestDead()
{
	for (int i = 0; i < static_cast<signed int>(m_vParticles.size()); ++i) {
		//If the particle is dead, return it
		if (m_vParticles[i]->IsDead()) {
			return (m_vParticles[i]);
		}
	}

	//If the loop ends and no particle found, then don't spawn any
	return nullptr;
}

//Pre fill the array with new particles
void 
ParticleEmitter::PreWarmArray(int maxParticles)
{
	for (int i = 0; i < maxParticles; ++i) {
		m_vParticles.push_back(new Particle());
		m_vParticles.back()->PreCreate(
			&m_pStartColor,
			m_lifeTime,
			0, 0,
			(m_particleSize != -999 ? m_particleSize : static_cast<int>(Calc::Random(5.0, 10.0)))
		);
	}
}
