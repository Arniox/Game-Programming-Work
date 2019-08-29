#pragma once

#include "Article.h"
#include "entity.h"
#include "Walls.h"

//Libraries
#include <cmath>

class PhysicsEngine
{
public:
	PhysicsEngine();
	~PhysicsEngine();

	//Collisions
	bool CheckEntityCollisions(Article* character, Article* entity);
	bool CheckWallCollisions(Article* character, Walls* wall);

	//Process Article Movement
	void ProcessArticle(float deltaTime, int currentMovementState, float & mo_f_InitialVelocityX, float& mo_f_InitialVelocityY, float & mo_f_MaxMovementSpeed, float & x, float & y);

protected:
	//Intersection checking
	bool CheckWallIntersection(Article& character, Walls& wall);
	//Intersection threshold
	int mo_f_threshold = 5000;

private:
	//Seperate processes
	void ProcessXAxisMotion(float deltaTime, int currentMovementState, float & mo_f_BurstX, float & mo_f_MaxMovementSpeed, float & x, float & y);
	void ProcessYAxisMotion(float deltaTime, float& mo_f_BurstY, float & x, float & y);

	//Constants
	//Friction Coefficents
	const float mx_f_staticFrictionCoefficient = 0.90f;
	const float mx_f_frictionInAir = 1.0f;
	float mx_f_currentFrictionCoe;
	//Base Variables
	const float mx_f_gravity = 9.81f;

	//Angle of surface that an entity collides with
	const float mx_f_maxFallAngle = 45;
	float mx_f_angleOfSurfaceEntityCollide;

	//Entity Variables
	float mx_f_accelerationX;
	float mx_f_accelerationY;
	float mx_f_finalVelocityX;
	float mx_f_finalVelocityY;

	//Collision
	bool mx_b_InAir;
	float mx_f_futureY;
};

