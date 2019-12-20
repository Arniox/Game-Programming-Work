
//Locals
#include "IniParser.h"
#include "LevelLoader.h"
#include "BitMapReader.h"
#include "Calc.h"
#include "sprite.h"
#include "game.h"
#include "backbuffer.h"
#include "Level.h"
#include "logmanager.h"

#include "DebugSquare.h"
#include "Building.h"
#include "FinishLine.h"

LevelLoader::LevelLoader(b2World* world, float _M2P, float _P2M)
: drawLayer()
, collisionLayer()
, m_pWorld(nullptr)
{
	drawLayer = new BitMapReader();
	collisionLayer = new BitMapReader();
	m_pWorld = world;
	M2P = _M2P;
	P2M = _P2M;
}
LevelLoader::~LevelLoader()
{
	DestroyLayers();

	for (std::vector<Sprite*>::iterator it = m_pDrawLayerObjects.begin(); it != m_pDrawLayerObjects.end(); ++it) {
		delete (*it);
		(*it) = nullptr;
	}
	m_pDrawLayerObjects.clear();
	m_pDrawLayerObjects.shrink_to_fit();

	for (std::vector<CollideableObject*>::iterator it = m_pCollideableObjects.begin(); it != m_pCollideableObjects.end(); ++it) {
		delete static_cast<DebugSquare*>((*it));
		(*it) = nullptr;
	}
	m_pCollideableObjects.clear();
	m_pCollideableObjects.shrink_to_fit();

	DestroySprites();
}

void 
LevelLoader::CreateLevel(std::string map, IniParser* levelsIni)
{
	CreateLevel(map, levelsIni, nullptr);
}

void
LevelLoader::CreateLevel(std::string map, IniParser* levelsIni, Level* level)
{
	LogManager::Title("Creating Level - Forming Key Data");
	FormData(levelsIni);
	LogManager::Title("Creating Level - Forming Sprites");
	FormSprites(levelsIni);

	SPRITE_SIZE = levelsIni->GetValueAsInt("TileData", "tileSize");

	LogManager::Title("Creating Level - Getting Draw & Collision Layer");
	std::string drawLayerImage = levelsIni->GetValueAsString(map, "drawLayer");
	std::string collisionLayerImage = levelsIni->GetValueAsString(map, "collisionLayer");

	if (!(drawLayerImage == "ERROR" || collisionLayerImage == "ERROR")) {
		drawLayer->LoadTexture(drawLayerImage);
		collisionLayer->LoadTexture(collisionLayerImage);
	}

	ProcessDrawLayer();
	ProccessCollisionLayer(level);

	DestroyLayers();
}

void 
LevelLoader::FormData(IniParser* levelsIni)
{
	const int bufferZeroTo = 3;
	const int stoiBase = 16;
	int i = 0;

	for (i = 0; i < T_SIZE; ++i) {
		//Get value
		std::string iniNum = levelsIni->GetValueAsString("Keys", std::to_string(i));
		//Split
		std::string s_1 = iniNum.substr(0, 2);
		std::string s_2 = iniNum.substr(2, 2);
		std::string s_3 = iniNum.substr(4, 2);

		m_pTilesData[i] = std::stoi(std::to_string(std::stoi(s_1, nullptr, stoiBase)) + 
									std::to_string(std::stoi(s_2, nullptr, stoiBase)) +
									std::to_string(std::stoi(s_3, nullptr, stoiBase)));
	}

	for (i = 0; i < CT_SIZE; ++i) {
		//Get value
		std::string iniNum = levelsIni->GetValueAsString("CollisionKeys", std::to_string(i));
		//Split
		std::string s_1 = iniNum.substr(0, 2);
		std::string s_2 = iniNum.substr(2, 2);
		std::string s_3 = iniNum.substr(4, 2);

		m_pCollisionData[i] = std::stoi(std::to_string(std::stoi(s_1, nullptr, stoiBase)) +
										std::to_string(std::stoi(s_2, nullptr, stoiBase)) +
										std::to_string(std::stoi(s_3, nullptr, stoiBase)));
	}

}

void 
LevelLoader::FormSprites(IniParser* levelsIni)
{
	//Pre generate sprites
	for (int i = 0; i < T_SIZE; ++i)
	{
		std::string spriteName = levelsIni->GetValueAsString("Tiles", std::to_string(i));
		m_pTileSprites[i] = Game::GetBackBuffer()->CreateSprite(spriteName.c_str());
	}
}

void 
LevelLoader::DestroySprites()
{
	for (int i = 0; i < T_SIZE; ++i)
	{
		delete m_pTileSprites[i];
		m_pTileSprites[i] = nullptr;
	}
}

void 
LevelLoader::DestroyLayers()
{
	delete drawLayer;
	drawLayer = nullptr;

	delete collisionLayer;
	collisionLayer = nullptr;

	memset(m_pTilesData, 0, sizeof(m_pTilesData));
	memset(m_pCollisionData, 0, sizeof(m_pCollisionData));
}

void
LevelLoader::ProcessDrawLayer()
{
	std::vector<std::vector<int>>* drawVector = drawLayer->GetPixelArray();

	for (int y = 0; y < static_cast<signed int>(drawVector->size()); ++y) {
		for (int x = 0; x < static_cast<signed int>(drawVector->at(y).size()); ++x) {
			//Find color and match to key
			if (drawVector->at(y).at(x) == m_pTilesData[TILES_NOTHING]) {
				m_pDrawLayerObjects.push_back(Game::GetBackBuffer()->CopySprite(m_pTileSprites[TILES_NOTHING]));
			}
			else if (drawVector->at(y).at(x) == m_pTilesData[TILES_R_VERT]) {
				m_pDrawLayerObjects.push_back(Game::GetBackBuffer()->CopySprite(m_pTileSprites[TILES_R_VERT]));
			}
			else if (drawVector->at(y).at(x) == m_pTilesData[TILES_R_HORIZ]) {
				m_pDrawLayerObjects.push_back(Game::GetBackBuffer()->CopySprite(m_pTileSprites[TILES_R_HORIZ]));
			}
			else if (drawVector->at(y).at(x) == m_pTilesData[TILES_RIGHT_TOP_TURN]) {
				m_pDrawLayerObjects.push_back(Game::GetBackBuffer()->CopySprite(m_pTileSprites[TILES_RIGHT_TOP_TURN]));
			}
			else if (drawVector->at(y).at(x) == m_pTilesData[TILES_LEFT_TOP_TURN]) {
				m_pDrawLayerObjects.push_back(Game::GetBackBuffer()->CopySprite(m_pTileSprites[TILES_LEFT_TOP_TURN]));
			}
			else if (drawVector->at(y).at(x) == m_pTilesData[TILES_RIGHT_BOTTOM_TURN]) {
				m_pDrawLayerObjects.push_back(Game::GetBackBuffer()->CopySprite(m_pTileSprites[TILES_RIGHT_BOTTOM_TURN]));
			}
			else if (drawVector->at(y).at(x) == m_pTilesData[TILES_LEFT_BOTTOM_TURN]) {
				m_pDrawLayerObjects.push_back(Game::GetBackBuffer()->CopySprite(m_pTileSprites[TILES_LEFT_BOTTOM_TURN]));
			}
			else if (drawVector->at(y).at(x) == m_pTilesData[TILES_ROAD_INTERSECTION]) {
				m_pDrawLayerObjects.push_back(Game::GetBackBuffer()->CopySprite(m_pTileSprites[TILES_ROAD_INTERSECTION]));
			}
			else if (drawVector->at(y).at(x) == m_pTilesData[TILES_PARK_GRASS]) {
				m_pDrawLayerObjects.push_back(Game::GetBackBuffer()->CopySprite(m_pTileSprites[TILES_PARK_GRASS]));
			}
			else {
				continue;
			}

			//Set the sprites X and Y
			m_pDrawLayerObjects.back()->SetX(static_cast<int>(Calc::map(x, 0, 128, -64 * SPRITE_SIZE, 64 * SPRITE_SIZE)));
			m_pDrawLayerObjects.back()->SetY(static_cast<int>(Calc::map(y, 0, 128, -64 * SPRITE_SIZE, 64 * SPRITE_SIZE)));
		}
	}
	
	//Clear now, no longer used sprites
	DestroySprites();
}

void
LevelLoader::ProccessCollisionLayer(Level* level)
{
	std::vector<std::vector<int>>* collisionVector = collisionLayer->GetPixelArray();

	for (int y = 0; y < static_cast<signed int>(collisionVector->size()); ++y)
	{
		for (int x = 0; x < static_cast<signed int>(collisionVector->at(y).size()); ++x)
		{

			if (collisionVector->at(y).at(x) == m_pCollisionData[CT_DEBUG])
			{
				m_pCollideableObjects.push_back(new DebugSquare(m_pWorld,
					64 + static_cast<int>(Calc::map(x, 0, 128, -64 * SPRITE_SIZE, 64 * SPRITE_SIZE)),
					64 + static_cast<int>(Calc::map(y, 0, 128, -64 * SPRITE_SIZE, 64 * SPRITE_SIZE)),
					4.0, M2P, P2M)
				);
			}
			else if (collisionVector->at(y).at(x) == m_pCollisionData[CT_END])
			{
				if (level != nullptr) {
					int xPos = static_cast<int>(Calc::map(x, 0, 128, -64 * SPRITE_SIZE, 64 * SPRITE_SIZE));
					int yPos = static_cast<int>(Calc::map(y, 0, 128, -64 * SPRITE_SIZE, 64 * SPRITE_SIZE));
					level->SetEndPoint(static_cast<float>(xPos + 64) * P2M, static_cast<float>(yPos + 64) * P2M);
					m_pCollideableObjects.push_back(new FinishLine(m_pWorld, 64 + xPos, 64 + yPos, M2P, P2M));
				}
			}
			else if (collisionVector->at(y).at(x) == m_pCollisionData[CT_BUILDING])
			{
				m_pCollideableObjects.push_back(new Building(m_pWorld,
					64 + static_cast<int>(Calc::map(x, 0, 128, -64 * SPRITE_SIZE, 64 * SPRITE_SIZE)),
					64 + static_cast<int>(Calc::map(y, 0, 128, -64 * SPRITE_SIZE, 64 * SPRITE_SIZE)),
					M2P, P2M)
				);
			}
		}
	}
}
