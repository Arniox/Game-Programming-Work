#define _CRTDBG_MAP_ALLOC
#include "WallController.h"

WallController::WallController()
{
}
WallController::~WallController()
{
	for (int i = 0; i < static_cast<signed int>(walls.size()); ++i) {
		delete walls.at(i);
		walls.at(i) = nullptr;
	}

	walls.clear();
	walls.shrink_to_fit();
}

void
WallController::Initialise(IniParser* iniParser, BackBuffer* m_pBackBuffer, double screenWidth, double screenHeight) {
	//LEVEL SCANNING FOR WALLS - STUB
	for (int i = 0; i < 8; ++i) {
		walls.push_back(new Walls());
	}

	//Manually set up walls for now
	//backbuffer, angle, length, height, x, y
	walls.at(0)->CreateWall(iniParser, m_pBackBuffer, screenWidth - 1, 26.0, 0.0, screenHeight - 27.0); //bottom
	walls.at(1)->CreateWall(iniParser, m_pBackBuffer, 26.0, screenHeight-1, 0.0, 0.0); //left
	walls.at(2)->CreateWall(iniParser, m_pBackBuffer, 26.0, screenHeight - 1, screenWidth - 27.0, 0.0); //Right
	walls.at(3)->CreateWall(iniParser, m_pBackBuffer, screenWidth - 1, 26.0, 0.0, 0.0); //Top
	walls.at(4)->CreateWall(iniParser, m_pBackBuffer, screenWidth / 2, 26.0, 0.0, screenHeight * 0.83); //Other walls
	walls.at(5)->CreateWall(iniParser, m_pBackBuffer, screenWidth / 3.2, 26.0, screenWidth / 3, screenHeight * 0.66);
	walls.at(6)->CreateWall(iniParser, m_pBackBuffer, screenWidth / 3.5, 26.0, screenWidth / 1.8, screenHeight * 0.49);
	walls.at(7)->CreateWall(iniParser, m_pBackBuffer, screenWidth / 1.8, 26.0, 0.0, screenHeight * 0.32);
}

void
WallController::Draw(BackBuffer& m_pBackBuffer) {
	for (int i = 0; i < static_cast<signed int>(walls.size()); ++i) {
		walls.at(i)->Draw(m_pBackBuffer);
	}
}

std::vector<Walls*>*
WallController::GetWalls() {
	return &walls;
}
