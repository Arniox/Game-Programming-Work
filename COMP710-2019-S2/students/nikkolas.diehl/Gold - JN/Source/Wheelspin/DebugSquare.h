#ifndef __DEBUG_SQUARE_H__
#define __DEBUG_SQUARE_H__

#include "CollideableObject.h"

class b2World;
class b2Body;
class Sprite;
class BackBuffer;

class DebugSquare:
	public CollideableObject
{
public:
	DebugSquare(b2World* pWorld, int x, int y, double scale, float _M2P, float _P2M);
	~DebugSquare();
	void Draw(BackBuffer& backBuffer);
public:
	b2World* pWorld;
	b2Body* m_pBody;
	Sprite* m_pSprite;
	float M2P;
	float P2M;
};

#endif // !__DEBUG_SQUARE_H__