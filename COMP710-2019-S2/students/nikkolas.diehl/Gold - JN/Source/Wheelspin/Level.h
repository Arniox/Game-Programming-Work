#ifndef __LEVEL_H__
#define __LEVEL_H__

#include "State.h"
#include "Box2D/Box2D.h"
#include "Constants.h"

#include <vector>
#include <SDL.h>

struct b2Vec2;
class DebugDraw;
class Car;
class Wheel;
class Camera;
class LevelsManager;
class DebugSquare;
class Arrow;
class GUI;

class Level :
	public State
{
public:
	Level();
	~Level();
	void Proccess(float deltaTime);
	void Draw(BackBuffer & backBuffer);
	void ProcessControls();
	void SetupWorld();
	void Pause();
	void SetEndPoint(float x, float y);

private:
	Camera* m_pCam;
	BackBuffer* m_pBackBuffer;
	DebugDraw* m_pDebugDrawer;
	b2World* m_pWorld;
	Arrow* m_pArrow;
	LevelsManager* m_pLevelsManager;
	Car* m_pCar;
	GUI* m_pPauseGUI;
	GUI* m_pStatsGUI;
	GUI* m_pWinScreen;
	std::vector<DebugSquare*> m_boxes;

	bool m_debugDraw;
	bool m_paused;
	bool m_gameWon;
	int m_carControl;
	float m_accelerator;
	float m_turnAmount;
	float M2P;
	float P2M;
	float m_timeTaken;
	float m_distanceToFinish;
	b2Vec2 m_endPoint;
};

#endif // !__LEVEL_H__