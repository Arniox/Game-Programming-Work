#ifndef __COLLIDEABLE_OBJECT_H__
#define __COLLIDEABLE_OBJECT_H__


class b2World;
class b2Body;
class Sprite;
class BackBuffer;

class CollideableObject
{
public:
	CollideableObject(b2World* pWorld, int x, int y, double scale);
	CollideableObject();
	~CollideableObject();
	virtual void Draw(BackBuffer& backBuffer);

protected:
	b2Body* m_pBody;
	Sprite* m_pSprite;
	float M2P;
	float P2M;
};

#endif // !__COLLIDEABLE_OBJECT_H__