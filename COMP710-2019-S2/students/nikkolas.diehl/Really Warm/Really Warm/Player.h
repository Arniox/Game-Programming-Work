#pragma once

#include "backbuffer.h"
#include "entity.h"
#include "inputhandler.h"
#include "WallController.h"
#include "Walls.h"
#include "inputhandler.h"

//Libraries
#include <string>
#include <cassert>

class Player :
	public Entity
{
public:
	Player();
	~Player();

	void CreatePlayer(BackBuffer* m_pBackBuffer, double x, double y);
	void Process(InputHandler* inputHandler);

private:
	int mx_i_health;
};

