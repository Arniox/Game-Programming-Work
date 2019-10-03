#define _CRTDBG_MAP_ALLOC
#include "EntityProcessingUnit.h"

EntityProcessingUnit::EntityProcessingUnit()
: screenBoundry()
, totalEntityCount(0)
, physicsEngine()
, mo_v_AllEntities()
, anythingAgainstWall(false)
, player(false)
, enemy(false)
, playerOrEnemy(false)
, weaponMain(false)
, bulletMain(false)
{
}
EntityProcessingUnit::~EntityProcessingUnit()
{
	delete physicsEngine;
	physicsEngine = nullptr;

	mo_v_AllEntities->clear();
	mo_v_AllEntities->shrink_to_fit();
	mo_v_AllEntities = nullptr;
}

void
EntityProcessingUnit::Initialise(std::vector<Entity*>* allEntites)
{
	//Copy refrences
	mo_v_AllEntities = allEntites;
	totalEntityCount = static_cast<signed int>(mo_v_AllEntities->size());

	//Init the physics engine
	physicsEngine = new PhysicsEngine();
}

void
EntityProcessingUnit::Process(QuadTree* quadTree, float deltaTime, int screenWidth, int screenHeight)
{
	processingCount = 0;

	for (entityIter = mo_v_AllEntities->begin(); entityIter != mo_v_AllEntities->end(); ++entityIter)
	{
		//Bind the entity to the screen. If outside, kill immediately.
		physicsEngine->BindEntity(screenWidth, screenHeight, deltaTime, (*entityIter));

		//If the entity is dead, do not process
		if ((*entityIter)->IsDead() || (*entityIter)->GetMat()->doNotProcess || !(*entityIter)->GetMat()->constrained) {
			continue;
		}

		//For every single entity, first process their velocity with stepped time
		//Add in custom processing of player later for time slow
		double processedDeltaTime = deltaTime / TIME_STEP;
		std::vector<Entity*> found;

		for (int i = 0; i < TIME_STEP; ++i) {
			//If type is weapon, and it is attached, then don't process
			if ((*entityIter)->GetMat()->TYPE == Material::WEAPON) {
				if ((static_cast<Weapon*>((*entityIter)))->IsAttached()) {
					continue;
				}
			}

			//Query quadtree
			quadTree->Query(&(*(*entityIter)->GetCollisionBox()->IncreaseSize(10)), found, (*entityIter));

			//Process entity velocity
			physicsEngine->ProcessEntityVelocity(deltaTime, processedDeltaTime, *entityIter);
			
			//For each entity against every other entity
			for (againstIter = found.begin(); againstIter != found.end(); ++againstIter) {
				//Cancel check if optimisation checks
				if (ReturnOptimization((*entityIter), (*againstIter))) {
					continue;
				}

				//Otherwise compute collision
				RequestCollisionResolution((*entityIter), (*againstIter));
				processingCount++;
			}

			found.clear();

			//Now process each entities new position based on velocity and friction
			physicsEngine->ProcessEntityPosition(processedDeltaTime, *entityIter);
		}
	}
}

int 
EntityProcessingUnit::GetProcessingCount()
{
	return processingCount;
}

bool inline
EntityProcessingUnit::ReturnOptimization(Entity* entityFrom, Entity* entityAgainst)
{
	//If the entity is dead, do not process
	if (entityAgainst->IsDead() || entityAgainst->GetMat()->doNotProcess) {
		return true;
	}

	//Set up bools
	anythingAgainstWall = entityAgainst->GetMat()->TYPE == Material::WALL;
	player = entityFrom->GetMat()->TYPE == Material::PLAYER;
	enemy = entityFrom->GetMat()->TYPE == Material::ENEMY;
	playerOrEnemy = player || enemy;
	weaponMain = entityFrom->GetMat()->TYPE == Material::WEAPON;
	bulletMain = entityFrom->GetMat()->TYPE == Material::BULLET;

	//Continue if checking against itself
	if (entityFrom == entityAgainst) {
		return true;
	}
	//Continue if bullet is checking bullet
	if (bulletMain && entityAgainst->GetMat()->TYPE == Material::BULLET) {
		return true;
	}
	//Continue if player is checking enemy
	if (player && enemy) {
		return true;
	}
	//Continue if weapon is checking weapon
	if (weaponMain && entityAgainst->GetMat()->TYPE == Material::WEAPON) {
		return true;
	}
	//Continue if weapon is already attached
	if (entityAgainst->GetMat()->TYPE == Material::WEAPON) {
		if (static_cast<Weapon*>(entityAgainst)->IsAttached()) {
			return true;
		}
	}
	//Continue if weapon is checking bullet
	if ((weaponMain || entityAgainst->GetMat()->TYPE == Material::WEAPON)
		&& (bulletMain || entityAgainst->GetMat()->TYPE == Material::BULLET)) {
		return true;
	}

	return false;
}

void
EntityProcessingUnit::RequestCollisionResolution(Entity* entityFrom, Entity* entityAgainst)
{
	//Otherwise compute collision
	if (physicsEngine->DetectCollisions(entityFrom, entityAgainst))
	{
		//Now check the two entity types and resolve collisions
		if (anythingAgainstWall) {
			//If anything is colliding against a wall then resolve collision
			physicsEngine->ResolveCollisions(entityFrom, entityAgainst);

			//If bullet and collide, decrement the bounce
			if (bulletMain) {
				static_cast<Bullet*>(entityFrom)->DecrementBounce(TIME_STEP);
			}
		}
		else if (playerOrEnemy && entityAgainst->GetMat()->TYPE == Material::WEAPON) {
			//If the player is already attached to something else then don't even check
			if (!static_cast<Player*>(entityFrom)->alreadyAttached) {
				//If the weapon is not on the pickup delay
				if (!static_cast<Weapon*>(entityAgainst)->pleaseDoNotInteract) {
					static_cast<Weapon*>(entityAgainst)->AttachTo(entityFrom);
				}
			}
		}
		else if (playerOrEnemy && entityAgainst->GetMat()->TYPE == Material::BULLET) {
			//If player or enemy is colliding with a bullet
			//If player or enemy health is higher than 0
			if (static_cast<Player*>(entityFrom)->GetHealth() > 0) {
				//If the bullet has damage
				double damageToDo = static_cast<Bullet*>(entityAgainst)->GetDamagePct();
				if (damageToDo > 0) {
					static_cast<Player*>(entityFrom)->NegateHealth(
						static_cast<int>(static_cast<Player*>(entityFrom)->GetMaxHealth() * (damageToDo / TIME_STEP))
					); //bullets collide multiple times per frame so do damage accordingly

					//Remove bullet damage since the bullet has now been used to deal damage
					static_cast<Bullet*>(entityAgainst)->DeleteDamage();
				}
			}
		}
	}
}
