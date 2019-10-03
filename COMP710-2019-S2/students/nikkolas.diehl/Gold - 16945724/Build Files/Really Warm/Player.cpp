#define _CRTDBG_MAP_ALLOC
#include "Player.h"

using namespace std;

Player::Player()
: mx_i_health(0)
, requestDeattachment(false)
, alreadyAttached(false)
, momentOfJump(false)
, MOVE_RIGHT(false)
, MOVE_LEFT(false)
, JUMP(false)
, THROW_GUN(false)
, aimingAngle(0)
, FIRE_GUN(false)
, weaponAttached(nullptr)
, playerPreviouslyAttached(false)
, movementForce(0.0)
{
}
Player::~Player()
{
	delete linearVelocity;
	linearVelocity = nullptr;
	delete force;
	force = nullptr;
	delete entityBox;
	entityBox = nullptr;
	delete mat;
	mat = nullptr;

	delete mo_sprite;
	mo_sprite = nullptr;
	delete mx_animatedSprite;
	mx_animatedSprite = nullptr;
}

bool 
Player::Initialise(AnimatedSprite * animatedSprite)
{
	assert(animatedSprite);
	mx_animatedSprite = animatedSprite;

	return (true);
}

void 
Player::Process(double deltaTime)
{
	//Give rightwards/leftwards momentum and animate
	if (MOVE_RIGHT) {
		force->x = movementForce;
		//Animated
		if (!mat->inAir) {
			//Set right animation
			mx_animatedSprite->SetCurrentPanel(RIGHT_ANIM);
		}
	}
	else if (MOVE_LEFT) {
		force->x = -movementForce;
		//Animated
		if (!mat->inAir) {
			//Set left animation
			mx_animatedSprite->SetCurrentPanel(LEFT_ANIM);
		}
	}

	//Give jump momentum
	if (JUMP) {
		force->y = -3000.0;
		momentOfJump = true;
	}

	//Throw weapon
	if (THROW_GUN) {
		requestDeattachment = true;
	}
	
	//Convert rad angle of entity to degrees for draw
	entityAngle = radiansToDegrees(entityAngleRad);

	//Process animated sprite
	mx_animatedSprite->Process(static_cast<float>(deltaTime));
}

void
Player::Draw(BackBuffer& backBuffer)
{
	assert(mx_animatedSprite);

	//Set angle of animated sprite
	mx_animatedSprite->SetAngle(entityAngle);

	//Set position of animated sprite
	mx_animatedSprite->SetX(static_cast<int>(entityBox->X()));
	mx_animatedSprite->SetY(static_cast<int>(entityBox->Y()));
	mx_animatedSprite->Draw(backBuffer, 255);
}

void 
Player::AttachTo(Entity* weapon)
{
	weaponAttached = weapon;
}

void 
Player::Deattach()
{
	weaponAttached = nullptr;
}

Entity* 
Player::GetAttachement()
{
	return weaponAttached;
}

int 
Player::GetMaxHealth()
{
	return mx_i_maxHealth;
}

int 
Player::GetHealth()
{
	return mx_i_health;
}

void 
Player::NegateHealth(int _newHealth)
{
	mx_i_health -= _newHealth;
}

bool
Player::DidJump()
{
	return momentOfJump;
}

double 
Player::GetAimingAngle()
{
	return aimingAngle;
}

bool 
Player::GetFireGun()
{
	return FIRE_GUN;
}

bool 
Player::GetPreviousAttachment()
{
	return playerPreviouslyAttached;
}

void 
Player::SetBaseStats(IniParser* iniParser, double scale)
{
	//Set material attributes
	mat->dead = false;
	mx_i_health = iniParser->GetValueAsInt("Player", "health");
	mx_i_maxHealth = mx_i_health; //health to be saved
	mat->constrained = iniParser->GetValueAsBoolean("Player", "constrained");
	mat->mass = iniParser->GetValueAsDouble("Player", "mass");
	mat->restitution = iniParser->GetValueAsDouble("Player", "restitution");

	//Set up physics
	linearVelocity = new Vector2();
	entityAngle = 0;
	anglularVelocity = 0;
	force = new Vector2();
	CalculateBoxIntertia(); //Set momentOfIntertia

	//Set up sprite sheet data
	mx_animatedSprite->SetSpriteScaler(scale);
	mx_animatedSprite->SetCurrentPanel(1); //Default not moving panel

	//Set attachement
	weaponAttached = nullptr;
}

void 
Player::ResetMovement()
{
	MOVE_RIGHT = false;
	MOVE_LEFT = false;
	JUMP = false;
	THROW_GUN = false;
	FIRE_GUN = false;

	force->x = 0;
	force->y = 0;
	requestDeattachment = false;

	//Set no animation
	mx_animatedSprite->SetCurrentPanel(NOTMOVING_ANIM);

	//No moment of jump
	momentOfJump = false;
}
