#pragma once
#include "entity.h"
class PlayerShip :
	public Entity
{
public:
	PlayerShip();
	~PlayerShip();

	bool shoot;
	bool moveRight;
	bool moveLeft;

	void PlayerShip::Process(float deltaTime, int screenWidth, int screenHeight, bool moveRight, bool moveLeft);
};

