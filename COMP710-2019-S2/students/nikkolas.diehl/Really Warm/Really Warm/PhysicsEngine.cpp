#include "PhysicsEngine.h"
#include "logmanager.h"
#include <string>

PhysicsEngine::PhysicsEngine()
	:mx_f_currentFrictionCoe(mx_f_staticFrictionCoefficient)
	,mx_f_angleOfSurfaceEntityCollide(0.0f)
	,mx_f_finalVelocityX(0.0f)
	,mx_f_finalVelocityY(0.0f)
	,mx_f_accelerationX(0.0f)
	,mx_f_accelerationY(0.0f)
	,mx_f_futureY(0.0f)
	,mx_b_InAir(true)
{
}
PhysicsEngine::~PhysicsEngine()
{
}

void 
PhysicsEngine::ProcessArticle(float deltaTime, int currentMovementState, float & mo_f_BurstX, float& mo_f_BurstY, float & mo_f_MaxMovementSpeed, float & x, float & y)
{
	//Proccess movement
	ProcessXAxisMotion(deltaTime, currentMovementState, mo_f_BurstX, mo_f_MaxMovementSpeed, x, y);
	ProcessYAxisMotion(deltaTime, mo_f_BurstY, x, y);
}

void
PhysicsEngine::ProcessXAxisMotion(float deltaTime, int currentMovementState, float & mo_f_BurstX, float & mo_f_MaxMovementSpeed, float & x, float & y) {
	
	//X AXIS PROCESSING

	//Acceleration = Max Movement Speed / deltaTime
	//Acceleration *= (-1 | 0 | 1)
	mx_f_accelerationX = mo_f_MaxMovementSpeed / deltaTime;
	if (!mx_b_InAir) {
		mx_f_accelerationX *= currentMovementState;
	}
	else {
		mx_f_accelerationX *= 0;
	}

	//If the anlge of the wall is higher than the max fall angle, then the article is falling
	if (abs(mx_f_angleOfSurfaceEntityCollide) > mx_f_maxFallAngle) {
		if (mx_f_angleOfSurfaceEntityCollide > 0) {      //Positive angle
			mx_f_accelerationX += cos(mx_f_angleOfSurfaceEntityCollide) * mx_f_gravity;
		}
		else if (mx_f_angleOfSurfaceEntityCollide < 0) { //Negative angle
			mx_f_accelerationX -= cos(mx_f_angleOfSurfaceEntityCollide) * mx_f_gravity;
		}
	}

	//Velocity = Initial Velocity + Acceleration * time
	//Velocity += Acceleration * deltaTime
	mx_f_finalVelocityX += mx_f_accelerationX * deltaTime;

	//Static Friction for simplicity
	mx_f_finalVelocityX *= mx_f_currentFrictionCoe;

	//Displacement = Initial Displacement + Velocity * time
	//Displacement += Velocity * deltaTime
	x += mx_f_finalVelocityX * deltaTime;
}

void
PhysicsEngine::ProcessYAxisMotion(float deltaTime, float& mo_f_BurstY, float & x, float & y) {
	
	//Y AXIS PROCESSING

	//Acceleration = Gravity / deltaTime
	mx_f_accelerationY = mx_f_gravity / deltaTime;
	
	//Velocity = Initial Velocity + Acceleration * time
	//Velocity += Acceleration * deltaTime
	mx_f_finalVelocityY += mx_f_accelerationY * deltaTime;

	//Reverse Force (set velocity to 0)
	if (!mx_b_InAir) {
		//Reverse motion as the ground pushes you upwards at the same rate gravity pushes you down
		mx_f_finalVelocityY = 0;

		//Jump force
		if (mo_f_BurstY != 0) {
			mx_f_finalVelocityY = -mo_f_BurstY;
		}
	}

	//Displacement = Initial Displacement + Velocity * time
	//Displacement += Velocity * deltaTime
	y += mx_f_finalVelocityY * deltaTime;

	mx_f_futureY = y + 0;
	mx_f_futureY += mx_f_finalVelocityY * deltaTime;

	
}

bool 
PhysicsEngine::CheckEntityCollisions(Article* character, Article* entity)
{
	//Character is 1, entity is 2
	//Object distance from eachother
	float objectDistance = (float)(sqrt(pow(entity->GetPositionX() - character->GetPositionX(), 2) + pow(entity->GetPositionY() - character->GetPositionY(), 2)) - character->GetSize() - entity->GetSize());
	//If collide, then return true
	if (objectDistance <= 0) {
		return true;
	}

	return false;
}

bool 
PhysicsEngine::CheckWallCollisions(Article* character, Walls* wall)
{
	//Check the wall collision first
	if (CheckWallIntersection((*character), (*wall))) {
		//Get angle of collision
		mx_f_angleOfSurfaceEntityCollide = wall->GetWallAngle();
		//Set current friction coefficient to surface contact with brick
		mx_f_currentFrictionCoe = mx_f_staticFrictionCoefficient;
		//Article is colliding with ground
		mx_b_InAir = false;
		return true;
	}
	else {
		//Set angle of collision to 0 as ground is flat when not colliding
		mx_f_angleOfSurfaceEntityCollide = 0;
		//Set current friction coefficient to in air
		mx_f_currentFrictionCoe = mx_f_frictionInAir;
		//Article is in air
		mx_b_InAir = true;
		return false;
	}
}

bool 
PhysicsEngine::CheckWallIntersection(Article & character, Walls & wall)
{
	

	//Get cross product of line-point intersection
	int dxc = (int)((mx_f_futureY + character.GetSize()) - wall.GetPoint1()[0]);
	int dyc = (int)((mx_f_futureY + character.GetSize()) - wall.GetPoint1()[1]);

	int dxl = (int)wall.GetDxl();
	int dyl = (int)wall.GetDyl();

	int cross = dxc * dyl - dyc * dxl;

	std::string strx = "Player X: " + std::to_string(character.GetPositionX());
	strx += "   Player Y: " + std::to_string(character.GetPositionY());
	strx += "   Future Y: " + std::to_string(mx_f_futureY);
	strx += "   dxc: " + std::to_string(dxc) + "   dyc: " + std::to_string(dyc);
	strx += "   dxl: " + std::to_string(dxl) + "   dyl: " + std::to_string(dyl);
	strx += "   Cross Product: " + std::to_string(cross);
	strx += "   Jump force: " + std::to_string(character.mo_f_BurstY);

	LogManager::GetInstance().Log(strx.c_str());

	//If and only if cross product is exactly equal to 0 then the point intersects with the line.
	//However this is problematic with ghosting and faster moving objects so as long as the absolute of the cross is
	//less than 7000 does a collision happen
	if (!(abs(cross) <= mo_f_threshold)) {
		return false;
	}
	else {
		if (abs(dxl) >= abs(dyl)) {
			return dxl > 0 ?
				wall.GetPoint1()[0] <= character.GetPositionX() && character.GetPositionX() <= wall.GetPoint2()[0] :
				wall.GetPoint2()[0] <= character.GetPositionX() && character.GetPositionX() <= wall.GetPoint1()[0];
		}
		else {
			return dyl > 0 ?
				wall.GetPoint1()[1] <= mx_f_futureY && mx_f_futureY <= wall.GetPoint2()[1] :
				wall.GetPoint2()[1] <= mx_f_futureY && mx_f_futureY <= wall.GetPoint1()[1];
		}
	}
}


