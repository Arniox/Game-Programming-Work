#ifndef __ENTITYPROCESSINGUNIT_H__
#define __ENTITYPROCESSINGUNIT_H__ 

//locals
#include "PhysicsEngine.h"
#include "Player.h"
#include "WallController.h"
#include "Walls.h"
#include "QuadTree.h"
#include "Weapon.h"

//Libraries
#include <vector>

class EntityProcessingUnit
{
public:
	EntityProcessingUnit();
	~EntityProcessingUnit();

	void Initialise(std::vector<Entity*>* allEntites);
	void Process(QuadTree* quadTree, float deltaTime, int screenWidth, int screenHeight);

	int GetProcessingCount();

protected:
	//Game speed scaler
	double mo_d_gameSpeedScaler = 1;

	//Process collisions
	bool inline ReturnOptimization(Entity* entityFrom, Entity* entityAgainst);
	void RequestCollisionResolution(Entity* entityFrom, Entity* entityAgainst);

	//All entity array
	std::vector<Entity*>::iterator entityIter;
	std::vector<Entity*>::iterator againstIter;
	std::vector<Entity*>* mo_v_AllEntities;

	//Quad tree functionality
	Rectangle* screenBoundry;
	int totalEntityCount;

	//Physics Engine
	PhysicsEngine* physicsEngine;

private:
	//Time step const
	const int TIME_STEP = 5;
	int processingCount = 0;

	//Bools for checking items againts eachother
	bool anythingAgainstWall;
	bool player;
	bool enemy;
	bool playerOrEnemy;
	bool weaponMain;
	bool bulletMain;

};

#endif

