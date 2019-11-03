#ifndef __FINISH_LINE_H__
#define __FINISH_LINE_H__

#include "CollideableObject.h"

class b2World;
class b2Body;
class Sprite;
class BackBuffer;

class FinishLine :
	public CollideableObject
{
public:
	FinishLine(b2World* pWorld, int x, int y, float _M2P, float _P2M);
	~FinishLine();
	void Draw(BackBuffer& backBuffer);
public:
	b2World* pWorld;
	b2Body* m_pBody;
	Sprite* m_pSprite;
	float M2P;
	float P2M;
};

#endif // !__FINISH_LINE_H__