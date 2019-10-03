#ifndef __ENTITY_H__
#define __ENTITY_H__

//Locals
#include "sprite.h"
#include "backbuffer.h"
#include "Vector2.h"
#include "Material.h"
#include "BoundingBox.h"

//Libraries
#include <cassert>
#include <cmath>

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
	Material* GetMat();

	//Get data
	BoundingBox* GetCollisionBox();
	Vector2* GetPos();
	Vector2* GetVelocity();
	double GetAngularVelocity() const;
	Vector2* GetForce();
	double* GetAngularVelocity();
	double* GetAngle();
	double* GetRadAngle();
	//Calcluate
	void CalculateBoxIntertia();

	void Kill();
	bool IsDead();

private:
	Entity(const Entity& entity);
	Entity& operator=(const Entity& entity);

protected:
	//Main Vectors
	BoundingBox* entityBox;		//Collision box with position vector
	Vector2* linearVelocity;	//Velocity Vector
	double anglularVelocity;	//Angular Velocity
	double entityAngle;
	double entityAngleRad;
	Vector2* force;				//Force Vector

	Material* mat;				//Material of entity
protected:
	Sprite* mo_sprite;
};

#endif //__ENTITY_H__
