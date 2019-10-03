#include "AIManager.h"
#include "AI.h"

AIManager::AIManager()
: aiParts()
{
}
AIManager::~AIManager()
{
	for (std::vector<AI*>::iterator it = aiParts.begin(); it != aiParts.end(); ++it) {
		delete (*it);
		(*it) = nullptr;
	}

	aiParts.clear();
	aiParts.shrink_to_fit();
}

void 
AIManager::Initialise(IniParser* m_iniParser, BackBuffer* m_pBackBuffer, int screenWidth, int screenHeight)
{
	int randomAICount = (rand() % 5) + 1;
	for (int i = 0; i < randomAICount; ++i) {
		//Spawn new AI
		aiParts.push_back(new AI());

		//For now, spawn him in a random area
		int xPos = (rand() % screenWidth - 26) + 13;
		int yPos = (rand() % screenHeight - 26) + 13;

		aiParts.back()->CreateAI(m_iniParser, m_pBackBuffer, xPos, yPos, screenWidth, screenHeight);
	}
}

void 
AIManager::Process(std::vector<Weapon*> weapons, Player* user, double deltaTime)
{
	for (std::vector<AI*>::iterator it = aiParts.begin(); it != aiParts.end(); ++it) {
		(*it)->ProcessAI(weapons, user, deltaTime);
	}
}

void 
AIManager::Draw(BackBuffer & backBuffer)
{
	for (std::vector<AI*>::iterator it = aiParts.begin(); it != aiParts.end(); ++it) {
		if (!((*it)->IsDead())) {
			static_cast<AI*>(*it)->Draw(backBuffer);
		}
	}
}

std::vector<AI*> 
AIManager::GetAI()
{
	return aiParts;
}

int 
AIManager::GetRemainingAI()
{
	int count = 0;
	for (std::vector<AI*>::iterator it = aiParts.begin(); it != aiParts.end(); ++it) {
		if ((*it)->IsDead()) {
			count++;
		}
	}
	
	return count;
}
