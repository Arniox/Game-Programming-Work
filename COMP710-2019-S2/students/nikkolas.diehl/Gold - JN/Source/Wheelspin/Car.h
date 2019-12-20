#ifndef __CAR_H__
#define __CAR_H__

class b2Body;
class Wheel;
class b2World;
class b2RevoluteJoint;
class BackBuffer;
class Sprite;
class ParticleEmitter;

#include "Box2D/Box2D.h"

#define NUM_WHEELS 4

class Car
{
public:
	Car(b2World* world, float xPos, float yPos, float width, float length, float angle, float _M2P, float _P2M);
	~Car();
	void SetPos(float x, float y, float angle, float P2M);
	void CreateCar(b2World* world, float xPos, float yPos, float width, float length, float angle);
	void Process(float deltaTime, int carControl, float acceleration, float turnAmount);
	void Draw(BackBuffer& backBuffer);

	float GetSpeed();
	void SetSpeed(float speed);
	b2Vec2 GetVelocity();

private:
	void ProcessParticles(float deltaTime, b2Vec2 speed);

public:
	b2Body* m_pBody;
protected:
	Wheel* m_wheels[NUM_WHEELS];
	Wheel* m_pFrontLeftWheel;
	Wheel* m_pFrontRightWheel;
	Wheel* m_pBackLeftWheel;
	Wheel* m_pBackRightWheel;
private:
	Sprite* m_pSprite;
	ParticleEmitter* m_pTailSmoke;
	ParticleEmitter* m_pTireTracksLeft;
	ParticleEmitter* m_pTireTracksRight;

	float m_maxSpeed;
	float m_maxReverseSpeed;
	float m_maxWheelAngle;
	float m_maxPower;

	float m_speed;
	float m_reverseSpeed;
	float m_maxForce;
	float m_maxImpulse;
	b2RevoluteJoint* m_fL;
	b2RevoluteJoint* m_fR;
	float M2P;
	float P2M;
};

#endif // !__CAR_H__