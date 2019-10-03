#pragma once
#ifndef __AIMANAGER_H__
#define __AIMANAGER_H__

//Locals
#include "iniParser.h"
#include "entity.h"
#include "Player.h"
#include "Weapon.h"
#include "AI.h"

//Libraries
#include <vector>

class AIManager
{
public:
	AIManager();
	~AIManager();

	void Initialise(IniParser* m_iniParser, BackBuffer* m_pBackBuffer, int screenWidth, int screenHeight);
	void Process(std::vector<Weapon*> weapons, Player* user, double deltaTime);
	void Draw(BackBuffer& backBuffer);

	std::vector<AI*> GetAI();
	int GetRemainingAI();

private:
	std::vector<AI*> aiParts;
};

#endif

