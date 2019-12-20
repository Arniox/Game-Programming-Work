#ifndef __BUILDING_H__
#define __BUILDING_H__

#include "CollideableObject.h"

class b2World;
class b2Body;
class Sprite;
class BackBuffer;

class Building :
	public CollideableObject
{
public:
	Building(b2World* pWorld, int x, int y, float _M2P, float _P2M);
	~Building();
	void Draw(BackBuffer& backBuffer);
public:
	b2World* pWorld;
	b2Body* m_pBody;
	Sprite* m_pSprite;
	float M2P;
	float P2M;
};

#endif