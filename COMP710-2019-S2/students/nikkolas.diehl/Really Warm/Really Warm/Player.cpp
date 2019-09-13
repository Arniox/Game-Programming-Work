#include "Player.h"

using namespace std;

Player::Player()
: mx_i_health(0)
{
}
Player::~Player()
{
	delete linearVelocity;
	delete force;

	delete entityBox;
	entityBox = 0;
}

void 
Player::CreatePlayer(BackBuffer* backBuffer, double x, double y)
{
	entityBox = new Rectangle(0, 0, 0, 0);

	Initialise(backBuffer->CreateSprite("assets/Sprites/playership.png"));
	SetSize(mo_sprite->GetWidth(), mo_sprite->GetHeight());
	SetCenter(x, y);

	mx_i_health = 100;
	mat->isKinematic = false;
	mat->movementSpeedLimit = 1.7;
	mat->mass = 60;
	mat->restitution = 0.2;

	//Set up
	linearVelocity = new Vector2();
	entityAngle = degreesToRadians(0);
	anglularVelocity = 0;
	force = new Vector2();
	CalculateBoxIntertia(); //Set momentOfIntertia
}

void 
Player::Process(InputHandler* inputHandler)
{
	//Check left/right keys pressed
	if (inputHandler->GetKeyBoardLayout("d")) {
		//RIGHT
		force->x = 50.0;
	}
	else if (inputHandler->GetKeyBoardLayout("a")) {
		//LEFT
		force->x = -50.0;
	}
	else {
		//NOT_MOVING
		force->x = 0.0;
	}

	//Jump
	if (inputHandler->GetKeyBoardLayout("space")) {
		force->y = -300.0;
	}
	else {
		force->y = 0;
	}
	
	mo_sprite->SetAngle(radiansToDegrees(entityAngle));
}