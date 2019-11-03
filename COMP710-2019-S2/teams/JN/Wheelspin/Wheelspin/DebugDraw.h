#ifndef __DEBUG_DRAW_H__
#define __DEBUG_DRAW_H__

#include "Box2D/Box2D.h"
#include "Constants.h"
#include "game.h"

class BackBuffer;

class DebugDraw
{
public:
	DebugDraw(float _M2P, float _P2M);
	~DebugDraw();

	bool Init(b2World* world, BackBuffer* backBuffer);
	void Draw();

	void DrawPolygon(b2Vec2* points, int numPoints);
	void RotateTranslate(b2Vec2 & vector, const b2Vec2 & center, float angle);

private:
	b2World* m_pWorld;
	BackBuffer* m_pBackBuffer;
	b2Vec2 m_points[24];
	float M2P;
	float P2M;
};

#endif // !__DEBUG_DRAW_H__