#pragma once
#ifndef __AI_H__
#define __AI_H__

//Locals
#include "Player.h"
#include "User.h"
#include "Weapon.h"

//Libraries
#include <vector>

class AI:
	public Player
{
public:
	AI();
	~AI();

	void CreateAI(IniParser* iniParser, BackBuffer* m_pBackBuffer, double x, double y, int screenWidth, int screenHeight);
	void ProcessAI(std::vector<Weapon*> weapons, Player* user, double deltaTime);

private:
	enum AIStates {
		HOLDING_NOTHING,
		HOLDING_GUN,
		NOT_CARING
	};
	AIStates currentState;

	Entity* FindClosestWeapon(std::vector<Weapon*> weapons);
	double DistanceToUser(Player* user);

	double accuracyOffset;

	//Try counter
	double tryCounter;

	//Screensize
	int windowWidth;
	int windowHeight;

	//Slower force
};

#endif

