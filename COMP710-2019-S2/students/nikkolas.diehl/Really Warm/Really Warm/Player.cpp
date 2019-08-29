#include "Player.h"
#include "inputhandler.h"
#include "Walls.h"

//Libraries
#include <string>

using namespace std;

Player::Player()
	:mo_i_health(0)
{
}
Player::~Player()
{
	delete this;
}

void 
Player::Initialise(BackBuffer * m_pBackBuffer)
{
	Entity::Initialise(m_pBackBuffer->CreateSprite("assets/Sprites/playership.png"));
	physicsEngine = new PhysicsEngine();

	mo_i_health = 100; //Health value in points
	mo_f_Mass = 110.0f;   //Mass in kilograms
	mo_f_Size = 32.0f;
	mo_f_MaxMovementSpeed = 30.71043f;
	mo_i_currentState = 0;
	mo_f_BurstX = 0;
	mo_f_BurstY = 0;
}

void 
Player::Process(InputHandler* inputHandler, Walls* walls, float deltaTime)
{
	//Check left/right keys pressed
	if (inputHandler->GetKeyBoardLayout("d")) {
		//RIGHT
		mo_i_currentState = 1;
	}
	else if (inputHandler->GetKeyBoardLayout("a")) {
		//LEFT
		mo_i_currentState = -1;
	}
	else {
		//NOT_MOVING
		mo_i_currentState = 0;
	}

	//Jump
	if (inputHandler->GetKeyBoardLayout("space")) {
		mo_f_BurstY = 350.0f;
	}
	else {
		mo_f_BurstY = 0;
	}

	physicsEngine->CheckWallCollisions(this, walls);
	physicsEngine->ProcessArticle(deltaTime, mo_i_currentState, mo_f_BurstX, mo_f_BurstY, mo_f_MaxMovementSpeed, m_x, m_y);
}
