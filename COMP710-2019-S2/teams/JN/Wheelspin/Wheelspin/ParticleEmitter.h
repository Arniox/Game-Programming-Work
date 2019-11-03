#ifndef __PARTICLEEMITTER_H__
#define __PARTICLEEMITTER_H__

//Locals
#include "Calc.h"
#include "Constants.h"
//Libraries
#include <SDL.h>
#include <vector>
#include <string>

//Forward Declerations
class Particle;
class BackBuffer;

class ParticleEmitter
{
public:
	ParticleEmitter();
	~ParticleEmitter();

	void SetDurationOfEmission();
	void SetDurationOfEmission(float in);
	void SetLooping();
	void SetLooping(bool in);
	void SetLifeTime();
	void SetLifeTime(float in);
	void SetStartSpeed();
	void SetStartSpeed(float in);
	void SetConeOfEmission();
	void SetConeOfEmission(float coneSize, float angle);
	void SetStartColor();
	void SetStartColor(SDL_Color& in);
	void SetEmissionRate();
	void SetEmissionRate(int in);
	void SetParticleSize();
	void SetParticleSize(bool random);
	void SetParticleSize(int in);
	void SetMaxParticles();
	void SetMaxParticles(int in);

	float GetConeSize();

	void Emission();
	void Process(double deltaTime, int x, int y);
	void Draw(BackBuffer& backBuffer);

protected:
	struct VariableFlags {
		bool durOfEmSet = false;
		bool loopSet = false;
		bool lifeTimeSet = false;
		bool startSpeedSet = false;
		bool coneOfEmSet = false;
		bool colSet = false;
		bool emRateSet = false;
		bool sizeSet = false;

		bool CheckAll() {
			return durOfEmSet && loopSet && lifeTimeSet && startSpeedSet && coneOfEmSet && colSet && emRateSet && sizeSet;
		}
	};

private:
	Particle* FindNewestDead();
	void PreWarmArray(int maxParticles);
	void Emit(int X, int Y);

private:
	std::vector<Particle*> m_vParticles;

	VariableFlags* m_variableFlags;

	int emitterX;
	int emitterY;

	float m_durationOfEmission;
	float m_durationCount;

	bool m_looping;
	float m_lifeTime;
	float m_velocity;
	float m_coneOfEmission;
	float m_halfCone;
	float m_angle;
	float m_anglePlusCone;
	float m_angleMinusCone;
	SDL_Color m_pStartColor;
	int m_emissionRate;
	int m_maxParticles;
	int m_particleSize;

	bool EMIITTING;
};

#endif

