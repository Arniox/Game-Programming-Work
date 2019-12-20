#ifndef __CAR_TEST2_H__
#define __CAR_TEST2_H__

#include "State.h"
#include "Box2D/Box2D.h"
#include "Constants.h"

#include <vector>

class DebugDraw;
class Car;
class Wheel;
class Camera;
struct b2Vec2;
class LevelsManager;
class DebugSquare;
class Arrow;
class GUI;
class ParticleEmitter;

class CarTest2 :
	public State
{
public:
	CarTest2();
	~CarTest2();
	void Proccess(float deltaTime);
	void Draw(BackBuffer & backBuffer);
	void ProcessControls();
	void SetupWorld();
	void Pause();

private:
	Camera* m_pCam;
	BackBuffer* m_pBackBuffer;
	DebugDraw* m_pDebugDrawer;
	b2World* m_pWorld;
	Arrow* m_pArrow;
	LevelsManager* m_pLevelsManager;
	Car* m_pCar;
	GUI* m_pPauseGUI;
	ParticleEmitter* m_pEmitterTest;

	std::vector<DebugSquare*> m_boxes;

	bool m_debugDraw;
	bool m_paused;
	int m_carControl;
	float m_accelerator;
	float m_turnAmount;
	float M2P;
	float P2M;
};

#endif // !__CAR_TEST_H__
