#ifndef __CAR_TEST_H__
#define __CAR_TEST_H__

#include "State.h"
#include "Box2D/Box2D.h"
#include "Constants.h"

#include <vector>

class DebugDraw;

class Car;
class Wheel;
class Sprite;
class Camera;

class DebugSquare;

class CarTest :
	public State
{
public:
	CarTest();
	~CarTest();
	void Proccess(float deltaTime);
	void Draw(BackBuffer & backBuffer);
	void ProcessControls();

	void SetupWorld();

private:
	BackBuffer* m_pBackBuffer;
	DebugDraw* m_pDebugDrawer;
	b2World* m_pWorld;

	Car* m_pCar;
	Sprite* m_pBall;
	Camera* m_pCam;
	std::vector<DebugSquare*> m_boxes;
	bool m_debugDraw;
	int m_carControl;
	float m_accelerator;
	float m_turnAmount;
	float M2P;
	float P2M;
};

#endif // !__CAR_TEST_H__
