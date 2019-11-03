
//Locals
#include "backbuffer.h"
#include "LevelsManager.h"
#include "IniParser.h"
#include "LevelLoader.h"
#include "game.h"
#include "sprite.h"
#include "Calc.h"
#include "CollideableObject.h"
#include "Level.h"
#include "logmanager.h"

LevelsManager::LevelsManager(b2World* world, Level* level, float _M2P, float _P2M)
: currentLevel(0)
, m_pLevel(nullptr)
, m_pLevelScene(nullptr)
{
	m_pLevelIni = new IniParser();
	std::string levelsSettingsFile = Game::GetIniFile()->GetValueAsString("Levels", "levelsIniFile");
	m_pLevelIni->LoadIniFile(levelsSettingsFile);
	m_pWorld = world;
	m_pLevelScene = level;

	M2P = _M2P;
	P2M = _P2M;
}
LevelsManager::~LevelsManager()
{
	delete m_pLevelIni;
	m_pLevelIni = nullptr;

	m_pWorld = nullptr;
	m_pLevelScene = nullptr;

	ClearLevel();
}

void 
LevelsManager::SetLevel(int levelNum)
{
	currentLevel = levelNum;

	AddLevel(currentLevel);
}

void 
LevelsManager::DrawLevel(BackBuffer& pBackBuffer)
{
	std::vector<Sprite*> drawLayerMap = m_pLevel->m_pDrawLayerObjects;

	for (int i = 0; i < static_cast<signed int>(drawLayerMap.size()); ++i) 
	{
		drawLayerMap[i]->Draw(pBackBuffer);
	}

	std::vector<CollideableObject*> collisionLayer = m_pLevel->m_pCollideableObjects;

	for (int i = 0; i < static_cast<signed int>(collisionLayer.size()); ++i) 
	{
		collisionLayer[i]->Draw(pBackBuffer);
	}
}

void
LevelsManager::AddLevel(int levelsNum)
{
	ClearLevel();

	m_pLevel = new LevelLoader(m_pWorld, M2P, P2M);
	std::string map = "Map " + std::to_string(levelsNum);
	m_pLevel->CreateLevel(map, m_pLevelIni, m_pLevelScene);
}

void
LevelsManager::ClearLevel()
{
	if (m_pLevel != nullptr) {
		delete m_pLevel;
		m_pLevel = nullptr;
	}
}
