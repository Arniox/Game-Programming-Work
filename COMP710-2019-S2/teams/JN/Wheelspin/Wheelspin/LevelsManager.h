#ifndef __LEVELSMANAGER_H__
#define __LEVELSMANAGER_H__

//Forward Decleration
class LevelLoader;
class IniParser;
class BackBuffer;
class b2World;
class Level;

//Libraries
#include <SDL.h>
#include <SDL_image.h>
#include <vector>

class LevelsManager
{
public:
	LevelsManager(b2World* world, Level* level, float _M2P, float _P2M);
	~LevelsManager();

	void SetLevel(int levelNum);
	void DrawLevel(BackBuffer& pBackBuffer);

private:
	void ClearLevel();
	void AddLevel(int levelNum);

private:
	LevelLoader* m_pLevel;
	b2World* m_pWorld;
	IniParser* m_pLevelIni;
	Level* m_pLevelScene;
	const int SPRITE_SIZE = 128;
	int currentLevel;

	float M2P;
	float P2M;
};

#endif

