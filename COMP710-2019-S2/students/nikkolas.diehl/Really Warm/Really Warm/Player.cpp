#include "Player.h"
#include "inputhandler.h"

//Libraries
#include <string>

using namespace std;

Player::Player()
	:mo_i_health(0)
{
}
Player::~Player()
{
	delete physicsEngine;
	delete this;
}

void 
Player::Initialise(BackBuffer * m_pBackBuffer)
{
	Entity::Initialise(m_pBackBuffer->CreateSprite("assets/Sprites/playership.png"));
	mp_i_SpriteHeight = m_pSprite->GetHeight();
	mp_i_SpriteWidth = m_pSprite->GetWidth();
	
	physicsEngine = new PhysicsEngine();

	mo_i_health = 100; //Health value in points
	mo_f_Mass = 110.0f;   //Mass in kilograms
	mo_f_Size = 32.0f;
	mo_f_MaxMovementSpeed = 1.71043f;
	mo_i_currentState = 0;
	mp_f_BurstX = 0;
	mp_f_BurstY = 0;
}

void 
Player::Process(InputHandler* inputHandler, WallController* wallController, float deltaTime)
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
		mp_f_BurstY = 350.0f;
	}
	else {
		mp_f_BurstY = 0;
	}

	for (int i = 0; i < static_cast<signed int>(wallController->GetWalls().size()); ++i) {
		physicsEngine->CheckWallCollisions(this, wallController->GetWalls().at(i));
	}
	physicsEngine->ProcessArticle(deltaTime, mo_i_currentState, mp_f_BurstX, mp_f_BurstY, mo_f_MaxMovementSpeed, m_x, m_y);
}
