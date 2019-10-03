#ifndef __PHYSICSENGINE_H__
#define __PHYSICSENGINE_H__

#include "entity.h"
#include "Walls.h"
#include "Vector2.h"
#include "logmanager.h"
#include "Material.h"

//Libraries
#include <cmath>
#include <string>

class PhysicsEngine
{
public:
	PhysicsEngine();
	~PhysicsEngine();

	//----------------Functions--------------------
	//Process entity velocity
	void ProcessEntityVelocity(double unprocessedDelta, double deltaTime, Entity* entity);

	//Bind an entity within the screen
	void BindEntity(int windowWidth, int windowHeight, double deltaTime, Entity* entity);

	//Detect collisions between two entities
	bool DetectCollisions(Entity* entity1, Entity* entity2);

	//Resolve collisions between two entities
	void ResolveCollisions(Entity* entity1, Entity* entity2);

	//Process entity position
	void ProcessEntityPosition(double deltaTime, Entity* entity);	

	//Log current object
	void LogCurrent(Entity* entity);

protected:

	//Add force
	void ApplyForce(Entity* character, Vector2* forceToAdd, double deltaTime);
	//Calculate Friction
	void CalculateFriction(Entity* character, double deltaTime);
	//Calculate Force and Torque
	void ComputeForceAndTorque(Entity* character);

private:
	//Friction Coefficents
	const double SKIN_METAL = 8;
	const double MAT_AIR = 0;
	double currentFriction;

	//Base Forces
	Vector2* gravity;
	Vector2* linearAcceleration;
	double angularAcceleration;
};

#endif

