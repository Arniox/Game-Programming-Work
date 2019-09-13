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
	gravity = new Vector2(0.0, 9.81);
	linearAcceleration = new Vector2();
}
PhysicsEngine::~PhysicsEngine()
{
	delete gravity;
}

void 
PhysicsEngine::ProcessEntity(float deltaTime,
	Entity* character, vector<Walls*> walls, 
	double screenWidth, double screenHeight)
{
	//double test = (*character->GetForce()).x;
	TimeStepProcess(deltaTime, character, walls, screenWidth, screenHeight, 100);
}

void
PhysicsEngine::TimeStepProcess(float deltaTime, 
	Entity* character, std::vector<Walls*> walls, 
	double screenWidth, double screenHeight,
	int timeStep) 
{
	double processedDeltaTime = deltaTime / timeStep;

	//Process entire physics engine multiple times per step with
	//a smaller deltaTime for more accuracy
	for (int i = 0; i < timeStep; ++i) {
		//Add forces back per frame
		ComputeForceAndTorque(character);
		ApplyForce(character, gravity);
		ApplyForce(character, character->GetForce());

		//Set angular acceleration
		angularAcceleration = character->GetMat()->torque / character->GetMat()->momentOfIntertia;

		//Add Velocity per frame for linear and angular
		character->GetVelocity()->Add(&(*linearAcceleration->ScaleMult(processedDeltaTime)));
		*character->GetAngularVelocity() += angularAcceleration * processedDeltaTime;

		//Calculate and resolve collisions
		DetectCollision(character, walls, processedDeltaTime);
		
		//Add displacement per frame for linear and angular
		character->GetPos()->Add(&(*character->GetVelocity()->ScaleMult(processedDeltaTime)));
		//*character->GetAngle() += *character->GetAngularVelocity() * processedDeltaTime;

		//Add friction
		CalculateFriction(character, processedDeltaTime);

		//Clear forces per frame
		linearAcceleration->Clear();
	}

	//Log
	//LogCurrent(character);
}

void
PhysicsEngine::ApplyForce(Entity* character, Vector2* forceToAdd)
{
	//Add acceleration to accleration
	//Vector2* scaledForce = &(*(forceToAdd->ScaleMult(character->GetMat()->mass)));
	linearAcceleration->Add(&(*forceToAdd->ScaleMult(character->GetMat()->mass)));
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
PhysicsEngine::DetectCollision(Entity* character, std::vector<Walls*> walls, double deltaTime)
{
	//Iterate through all walls
	vector<Walls*>::iterator wallIter;
	for (wallIter = walls.begin(); wallIter != walls.end(); ++wallIter) {
		//Refresh entity inAir state
		character->GetMat()->inAir = true;
		//Broad
		if (character->GetCollisionBox()->CheckCollision((*wallIter)->GetCollisionBox())) {
			//Update physics
			UpdatePhysicsResponse(character, (*wallIter));
		}
	}
}

void
PhysicsEngine::UpdatePhysicsResponse(Entity* character, Walls* wall) {
	if (character->GetMat()->isKinematic) {
		return;
	}

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
	
	{
		unique_ptr<Vector2> rv = move(*wall->GetVelocity() - *character->GetVelocity());
		unique_ptr<Vector2> normal = std::make_unique<Vector2>();

		normal->Copy(character->GetCollisionBox()->GetNormal());
		double contactVel = (*rv).Dot(&(*normal));

		//Character is in air when normal.y is not equal to 0
		if (normal->y != 0) {
			character->GetMat()->inAir = false;
		}

		//Do not resolve if velocities are seperating
		if (contactVel > 0) {
			return;
		}

		//Calculate resitution
		double e = min(character->GetMat()->restitution, wall->GetMat()->restitution);
		//Calculate impulse scalar
		double j = -(1.0 + e) * contactVel;
		j /= (character->GetMat()->mass == INFINITY ? 0 : character->GetMat()->mass) +
			(wall->GetMat()->mass == INFINITY ? 0 : wall->GetMat()->mass);

		//Apply impulse
		unique_ptr<Vector2> impulse = move((*normal).ScaleMult(j));
		//Change velocity
		if (character->GetMat()->mass != INFINITY) {
			character->GetVelocity()->Minus(&(*(*impulse).ScaleMult(character->GetMat()->mass)));
		}
		if (wall->GetMat()->mass != INFINITY) {
			wall->GetVelocity()->Add(&(*(*impulse).ScaleMult(wall->GetMat()->mass)));
		}
	}
	
}

void
PhysicsEngine::LogCurrent(Entity* character) 
{
	//Log all
	string output = "Character == Pos: " + character->GetPos()->ToString();
	output += " Linear Velocity: " + character->GetVelocity()->ToString();
	output += " Angular Velocity: " + to_string(*character->GetAngularVelocity());
	output += " Angle: " + to_string(*character->GetAngle());
	output += " Torque: " + to_string(character->GetMat()->torque);
	output += " Linear Acceleration: " + linearAcceleration->ToString();

	LogManager::GetInstance().Log(output.c_str());
}


