#define _CRTDBG_MAP_ALLOC
#include "PhysicsEngine.h"

#define min(a, b) (((a) < (b)) ? (a) : (b))
#define max(a, b) (((a) > (b)) ? (a) : (b))
#define degreesToRadians(angleDegrees) (angleDegrees * M_PI / 180.0)
#define radiansToDegrees(angleRadians) (angleRadians * 180.0 / M_PI)

using namespace std;

PhysicsEngine::PhysicsEngine()
: angularAcceleration(0.0)
, currentFriction(SKIN_METAL)
{
	gravity = new Vector2(0.0, 20);
	linearAcceleration = new Vector2();
}
PhysicsEngine::~PhysicsEngine()
{
	delete gravity;
	gravity = nullptr;
	delete linearAcceleration;
	linearAcceleration = nullptr;
}

//Process an entities velocity
void
PhysicsEngine::ProcessEntityVelocity(double unprocessedDelta, double deltaTime, Entity* entity)
{
	//Return immediatiely if entity type is wall or nothing
	if (entity->GetMat()->TYPE == Material::WALL || entity->GetMat()->TYPE == Material::NOTHING) {
		return;
	}
	else {
		//Compute torque and apply gravity force
		ComputeForceAndTorque(entity);
		ApplyForce(entity, gravity, unprocessedDelta);

		//If in air, apply character driven force
		//Seperate to starting velocity
		if (!entity->GetMat()->inAir) {
			ApplyForce(entity, entity->GetForce(), unprocessedDelta);
			currentFriction = SKIN_METAL;
		}
		else {
			currentFriction = MAT_AIR;
		}		

		//Set angular acceleration
		angularAcceleration = entity->GetMat()->torque / entity->GetMat()->momentOfIntertia;

		//Add velocity per frame for linear and angular
		entity->GetVelocity()->Add(&(*linearAcceleration->ScaleMult(deltaTime)));
		*entity->GetAngularVelocity() += angularAcceleration * deltaTime;		

		//Refresh the inAir check
		entity->GetMat()->inAir = true;
	}	
}

void  
PhysicsEngine::BindEntity(int windowWidth, int windowHeight, double deltaTime, Entity * entity)
{
	if (entity->GetCollisionBox()->X() > windowWidth ||
		entity->GetCollisionBox()->X() < 0 ||
		entity->GetCollisionBox()->Y() > windowHeight ||
		entity->GetCollisionBox()->Y() < 0) {

		entity->Kill();
	}
}

//Detect if two entities collide
bool
PhysicsEngine::DetectCollisions(Entity* entity1, Entity* entity2)
{
	//Are the two entities colliding
	if (entity1->GetCollisionBox()->CheckCollision(entity2->GetCollisionBox())) {
		//Yes they are
		return true;
	}

	//No they aren't
	return false;
}

//Resolve any known collision between two entities of the correct type
void
PhysicsEngine::ResolveCollisions(Entity* entity1, Entity* entity2)
{
	//-----------------Update physics------------------
	//Simplistic physics simulation. Simply negates velocity from itself and moves back by one pixel
	/*
	{
		unique_ptr<Vector2> normal = std::make_unique<Vector2>();
		normal->Copy(character->GetCollisionBox()->GetNormal());
		if (normal->x < 0 || normal->y < 0) {
			character->GetVelocity()->Add(&(*(*character->GetVelocity() * *normal)));
		}
		else {
			character->GetVelocity()->Minus(&(*(*character->GetVelocity() * *normal)));
		}
		character->GetPos()->Minus(&(*normal->ScaleDiv(2)));
	}
	*/

	//Physically calculated impulse direction change:

	//Calculate out contact velocity from relative velocity and normal of character
	unique_ptr<Vector2> rv = move(*entity2->GetVelocity() - *entity1->GetVelocity());
	unique_ptr<Vector2> normal = std::make_unique<Vector2>();

	normal->Copy(entity1->GetCollisionBox()->GetNormal());
	double contactVel = (*rv).Dot(&(*normal));

	//Do not resolve if velocities are seperating
	if (contactVel > 0) {
		return;
	}

	//Calculate resitution
	double e = max(entity1->GetMat()->restitution, entity2->GetMat()->restitution);
	//Calculate impulse scalar
	double j = -(1.0 + e) * contactVel;
	
	//if the entity have infinite mass, then only divide by masses at 0. Both entities can't have infinite mass and collide
	j /= (entity1->GetMat()->mass == INFINITY ? 0 : entity1->GetMat()->mass) +
		 (entity2->GetMat()->mass == INFINITY ? 0 : entity2->GetMat()->mass);

	//Apply impulse
	unique_ptr<Vector2> impulse = move((*normal).ScaleMult(j));

	if (impulse->y > 0) {
		entity1->GetMat()->inAir = false;
	}

	//Change velocity
	if (entity1->GetMat()->mass != INFINITY) {
		if (!entity2->GetMat()->isPhase) {
			entity1->GetVelocity()->Minus(&(*(*impulse).ScaleMult(entity1->GetMat()->mass)));
		}
	}
	if (entity2->GetMat()->mass != INFINITY) {
		entity2->GetVelocity()->Add(&(*(*impulse).ScaleMult(entity2->GetMat()->mass)));
	}
}

//Process an entities position with friction
void
PhysicsEngine::ProcessEntityPosition(double deltaTime, Entity* entity)
{
	//Add displacement per frame for linear and angular
	entity->GetPos()->Add(&(*entity->GetVelocity()->ScaleMult(deltaTime)));
	//*character->GetRadAngle() += *character->GetAngularVelocity() * processedDeltaTime;

	//Add friction
	CalculateFriction(entity, deltaTime);

	//Clear forces per frame
	linearAcceleration->Clear();
}

void
PhysicsEngine::ApplyForce(Entity* character, Vector2* forceToAdd, double deltaTime)
{
	//Add acceleration to accleration
	//Vector2* scaledForce = &(*(forceToAdd->ScaleMult(character->GetMat()->mass)));
	linearAcceleration->Add(&(*forceToAdd->ScaleDiv(deltaTime)));
}

void
PhysicsEngine::CalculateFriction(Entity* character, double deltaTime)
{
	//Calculate friction coe
	double frictionCoe = 1 / (1 + deltaTime * currentFriction);

	//Add friction to both angular and movement velocities
	character->GetVelocity()->x *= frictionCoe;
	*character->GetAngularVelocity() *= frictionCoe;
}

void
PhysicsEngine::ComputeForceAndTorque(Entity* character) {
	//Calculate torque
	character->GetMat()->torque = -((character->GetCollisionBox()->W() / 2) *
		(character->GetForce()->y * 100) -
		(character->GetCollisionBox()->H() / 2) *
		(character->GetForce()->x * 100));
}

void
PhysicsEngine::LogCurrent(Entity* character) 
{
	//Log all
	//Log for character data
	string output = "Character == Pos: " + character->GetPos()->ToString();
	output += " Linear Velocity: " + character->GetVelocity()->ToString();
	output += " Angular Velocity: " + to_string(*character->GetAngularVelocity());
	output += " Angle: " + to_string(*character->GetRadAngle());
	output += " Torque: " + to_string(character->GetMat()->torque);
	output += " Linear Acceleration: " + linearAcceleration->ToString();

	//Log for character inAir check
	string output2 = "inAir = " + to_string(character->GetMat()->inAir);

	LogManager::GetInstance().Log(output2.c_str());
	LogManager::GetInstance().DestroyInstance();
}


