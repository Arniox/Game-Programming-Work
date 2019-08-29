#pragma once
#include "backbuffer.h"
#include "entity.h"
#include "inputhandler.h"
#include "Article.h"
#include "PhysicsEngine.h"

//Libraries

class Player :
	public Article
{
public:
	Player();
	~Player();

	void Initialise(BackBuffer* m_pBackBuffer);
	void Process(InputHandler* inputHandler, Walls* walls, float deltaTime);

private:
	int mo_i_health;

	PhysicsEngine* physicsEngine;
};

