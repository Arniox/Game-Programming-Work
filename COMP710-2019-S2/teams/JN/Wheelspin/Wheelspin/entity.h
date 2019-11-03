#ifndef __ENTITY_H__
#define __ENTITY_H__

//Locals
class Sprite;
class BackBuffer;

//Libraries
#include <cassert>
#include "Box2D/Common/b2Math.h"

#define degreesToRadians(angleDegrees) (angleDegrees * M_PI / 180.0)
#define radiansToDegrees(angleRadians) (angleRadians * 180.0 / M_PI)

// Forward Declarations:
class BackBuffer;
class Sprite;

class Entity
{
public:
	//Main methods
	Entity();
	~Entity();

	bool Initialise(Sprite* sprite);

	void Process(float deltaTime);
	void Draw(BackBuffer& backBuffer, unsigned char alpha);

public:
	//Secondary methods

	//Position entity
	void SetPos(double x, double y);
	//Set width and height
	void SetSize(double width, double height);
	//Get sprite
	Sprite* GetSprite();

	void Kill();
	bool IsDead();

private:
	Entity(const Entity& entity);
	Entity& operator=(const Entity& entity);

protected:
	Sprite* m_pSprite;

	double m_entityAngle;
	double m_entityAngleRad;
};

#endif
