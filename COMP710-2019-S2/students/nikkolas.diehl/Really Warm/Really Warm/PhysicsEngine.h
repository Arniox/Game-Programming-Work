#pragma once

#include "entity.h"
#include "Walls.h"
#include "Vector2.h"
#include "logmanager.h"

//Libraries
#include <cmath>
#include <string>

class PhysicsEngine
{
public:
	PhysicsEngine();
	~PhysicsEngine();

	//Process Article Movement
	void ProcessEntity(float deltaTime, 
		Entity* character, std::vector<Walls*>* walls, 
		double screenWidth, double screenHeight);

protected:

	//Timestep
	void TimeStepProcess(float deltaTime,
		Entity* character, std::vector<Walls*>* walls, 
		double screenWidth, double screenHeight,
		int timeStep);

	//Add force
	void ApplyForce(Entity* character, Vector2* forceToAdd);
	//Calculate Friction
	void CalculateFriction(Entity* character, double deltaTime);
	//Calculate Force and Torque
	void ComputeForceAndTorque(Entity* character);

	//Calculate and resolve collisions
	void DetectCollision(Entity* character, std::vector<Walls*>* walls, double deltaTime);
	void UpdatePhysicsResponse(Entity* character, Walls* wall);

	//Log current object
	void LogCurrent(Entity* character);

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

