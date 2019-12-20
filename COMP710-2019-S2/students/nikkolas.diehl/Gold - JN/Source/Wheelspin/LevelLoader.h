#ifndef __LEVELLOADER_H__
#define __LEVELLOADER_H__

class BitMapReader;
class IniParser;
class Sprite;
class b2World;
class CollideableObject;
class Level;

//Libraries
#include <string>
#include <vector>
#include <SDL.h>
#include <SDL_image.h>

enum Tiles
{
	TILES_NOTHING,
	TILES_R_VERT,
	TILES_R_HORIZ,
	TILES_RIGHT_TOP_TURN,
	TILES_LEFT_TOP_TURN,
	TILES_RIGHT_BOTTOM_TURN,
	TILES_LEFT_BOTTOM_TURN,
	TILES_ROAD_INTERSECTION,
	TILES_PARK_GRASS,

	T_SIZE
};

enum Collision_Tiles
{
	CT_NOTHING,
	CT_START,
	CT_END,
	CT_DEBUG,
	CT_BUILDING,

	CT_SIZE
};

class LevelLoader
{
public:
	LevelLoader(b2World* world, float _M2P, float _P2M);
	~LevelLoader();

	void CreateLevel(std::string map, IniParser* levelsIni);
	void CreateLevel(std::string map, IniParser* levelsIni, Level* level);

public:
	std::vector<Sprite*> m_pDrawLayerObjects;
	std::vector<CollideableObject*> m_pCollideableObjects;

protected:
	void FormData(IniParser* levelsIni);
	void FormSprites(IniParser* levelsIni);

	void DestroySprites();
	void DestroyLayers();

	void ProcessDrawLayer();
	void ProccessCollisionLayer(Level* level);

private:
	BitMapReader* drawLayer;
	BitMapReader* collisionLayer;

	Sprite* m_pTileSprites[T_SIZE];

	int m_pTilesData[T_SIZE];
	int m_pCollisionData[CT_SIZE];

	int SPRITE_SIZE = 0;

	float M2P;
	float P2M;
	b2World* m_pWorld;
};

#endif

