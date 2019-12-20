#ifndef __TEST_H__
#define __TEST_H__

#include "State.h"

#include <Box2D/Box2D.h>

class Sprite;
class Wheel;
class DebugDraw;

class Test :
	public State
{
public:
	Test();
	~Test();
	void Proccess(float deltaTime);
	void Draw(BackBuffer & backBuffer);
	void ProcessControls();

	void addRect(int x, int y, int w, int h, bool dyn);

private:
	DebugDraw* m_pDebugDrawer;
	Sprite* left;
	Sprite* right;
	Sprite* lt;
	Sprite* rt;

	b2World* m_pWorld;
	b2Body* groundBody;
	b2BodyDef groundBodyDef;
	b2Body* Body;
	b2BodyDef ballBodyDef;
	b2PolygonShape groundBox;
	b2FixtureDef boxShapeDef;

	Wheel* wheel;

	Sprite* m_pBall;

	int m_centerX;
	int m_centerY;
	float M2P = 32.0f;
	float P2M = 1.0f / M2P;
	int delayCount;
};

#endif // !__TEST_H__