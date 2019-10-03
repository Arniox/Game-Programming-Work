#include "User.h"

using namespace std;

User::User()
{
}
User::~User()
{
}

void 
User::CreateUser(IniParser* iniParser, BackBuffer* m_pBackBuffer, double x, double y)
{
	entityBox = new BoundingBox();
	double scale = iniParser->GetValueAsDouble("Player", "scale");
	std::string spriteSheet = iniParser->GetValueAsString("Player", "spriteSheet");
	movementForce = iniParser->GetValueAsDouble("Player", "forceLimit");

	Initialise(m_pBackBuffer->CreateAnimatedSprite(spriteSheet.c_str(),
		FRAME_COUNT, PANNEL_COUNT, FRAME_SPEED));

	SetSize(mx_animatedSprite->GetFrameWidth() * scale, mx_animatedSprite->GetFrameHeight() * scale);
	SetPos(x, y);

	//Set type
	mat->TYPE = mat->PLAYER;
	mat->movementSpeedLimit = iniParser->GetValueAsDouble("Player", "movementSpeedLimit");

	//Set material attributes
	SetBaseStats(iniParser, scale);
}

void 
User::ProcessUser(InputHandler* inputHandler, double deltaTime)
{
	//Set previous values
	playerPreviouslyAttached = alreadyAttached;

	//Reset the movement variables
	ResetMovement();

	//Set aiming angle
	aimingAngle = GetPos()->AngleDeg(&(*inputHandler->GetMousePos()));

	//Moving right
	if (inputHandler->GetKeyBoardLayout("d")) {
		MOVE_RIGHT = true;
	}
	//Moving left
	if (inputHandler->GetKeyBoardLayout("a")) {
		MOVE_LEFT = true;
	}
	//Jumping
	if (inputHandler->GetKeyBoardLayout("space")) {
		JUMP = true;
	}
	//Throw Weapon
	if (inputHandler->GetKeyBoardLayout("q")) {
		THROW_GUN = true;
	}
	//Fire
	if (inputHandler->GetMouseClickStates()[0]) {
		FIRE_GUN = true;
	}

	//Process the entity
	Player::Process(deltaTime);
}
