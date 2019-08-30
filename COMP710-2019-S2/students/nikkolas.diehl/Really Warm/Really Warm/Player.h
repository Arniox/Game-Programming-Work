#pragma once
#include "backbuffer.h"
#include "entity.h"
#include "inputhandler.h"
#include "Article.h"
#include "PhysicsEngine.h"
#include "WallController.h"
#include "Walls.h"

//Libraries

class Player :
	public Article
{
public:
	Player();
	~Player();

	void Initialise(BackBuffer* m_pBackBuffer);
	void Process(InputHandler* inputHandler, WallController* wallController, float deltaTime);

private:
	int mo_i_health;

	PhysicsEngine* physicsEngine;
};

