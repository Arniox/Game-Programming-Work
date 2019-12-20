#ifndef __WHEEL_H__
#define __WHEEL_H__

class b2Body;
class b2World;
class BackBuffer;
class Sprite;

#include "Box2D/Box2D.h"

enum {
	CAR_LEFT = 0x1,
	CAR_RIGHT = 0x2,
	CAR_FORWARD = 0x4,
	CAR_BACKWARD = 0x8
};

class Wheel
{
public:
	Wheel(b2World* world, b2Body* carBody, float xPos, float yPos, float wheelWidth, float wheelHeight, bool revolving, bool powered, float _M2P, float _P2M);
	~Wheel();
	void CreateWheel(b2World* world, float xPos, float yPos, float wheelWidth, float wheelHeight, bool revolving, bool powered);
	void Init(float speed, float reverseSpeed, float maxForce, float maxImpulse);
	void Draw(BackBuffer& backBuffer);

	void Process(int carControl, float acceleration);
	void ProcessAcceleration(int carControl, float acceleration);
	void ProcessTurning(int carControl);
	b2Vec2 GetLateralVelocity();
	b2Vec2 GetForwardVelocity();
	void UpdateFriction();

	void SetAngle(float32 angle);
	b2Vec2 GetVelocity();
	b2Vec2 GetDirection();
	void RemoveLateralVelocity();
	b2Vec2 GetVelocityVector();
public:
	b2Body* m_pBody;
	b2Body* m_pCarBody;
	Sprite* m_pSprite;
	float m_speed;
	float m_reverseSpeed;
	float m_maxForce;
	float m_maxImpulse;
	float M2P;
	float P2M;

private:
	b2Vec2 forwardVec = b2Vec2(0.0f, 1.0f);
};

#endif // !__WHEEL_H__