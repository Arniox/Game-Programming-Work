#ifndef __CAMERA_H__
#define __CAMERA_H__

#include "Box2D/Box2D.h"
#include "Vector2.h"

class Camera
{
public:
	Camera(float _M2P, float _P2M);
	~Camera();
	void SetPos(int x, int y);
	void SetPos(b2Vec2 pos);
	bool InBounds(int x, int y);
private:
	void Update();
public:
	int x;
	int y;
	int m_screenCenterX;
	int m_screenCenterY;
	int m_maxX;
	int m_maxY;
	int m_minX;
	int m_minY;
	int m_paddingX;
	int m_paddingY;
	float M2P;
	float P2M;
};

#endif // !__CAMERA_H__