//File: Player.h
#ifndef __PLAYER_H__
#define __PLAYER_H__

//Locals
#include "iniParser.h"
#include "backbuffer.h"
#include "entity.h"
#include "inputhandler.h"
#include "WallController.h"
#include "Walls.h"
#include "inputhandler.h"
#include "animatedsprite.h"

//Libraries
#include <string>
#include <cassert>

class Player :
	public Entity
{
public:
	Player();
	~Player();

	void Draw(BackBuffer& m_pBackBuffer);

	void AttachTo(Entity* weapon);
	void Deattach();
	Entity* GetAttachement();
	bool alreadyAttached;
	bool requestDeattachment;

	int GetMaxHealth();
	int GetHealth();
	void NegateHealth(int _newHealth);
	bool DidJump();
	double GetAimingAngle();
	bool GetFireGun();

	//Get previous values
	bool GetPreviousAttachment();

protected:
	bool Initialise(AnimatedSprite* animatedSprite);
	void SetBaseStats(IniParser* iniParser, double scale);
	void Process(double deltaTime);

protected:
	int mx_i_health;
	int mx_i_maxHealth;

	//Reset all movement variables
	void ResetMovement();

	bool MOVE_RIGHT;
	bool MOVE_LEFT;
	bool JUMP;
	bool THROW_GUN;
	bool FIRE_GUN;

protected:
	//Sprite sheet information
	float const FRAME_SPEED = 1;

	int const FRAME_COUNT = 10;
	int const PANNEL_COUNT = 3;

	//Animation panel consts
	int const RIGHT_ANIM = 0;
	int const LEFT_ANIM = 2;
	int const NOTMOVING_ANIM = 1;

	AnimatedSprite* mx_animatedSprite;
	Entity* weaponAttached;

	//moment of jump
	bool momentOfJump;

	//Aiming angle
	double aimingAngle;

protected:
	//Previous data to store
	bool playerPreviouslyAttached;

	//force left and right
	double movementForce;
};

#endif

