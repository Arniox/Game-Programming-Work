#include "WallController.h"

WallController::WallController()
{
}
WallController::~WallController()
{
	for (int i = 0; i < static_cast<signed int>(walls.size()); ++i) {
		delete walls.at(i);
	}

	walls.clear();
}

void
WallController::Initialise(BackBuffer* m_pBackBuffer, double screenWidth, double screenHeight) {
	//LEVEL SCANNING FOR WALLS - STUB
	for (int i = 0; i < 5; ++i) {
		walls.push_back(new Walls());
	}

	//Manually set up walls for now
	//backbuffer, angle, length, height, x, y
	walls.at(0)->CreateWall(m_pBackBuffer, screenWidth - 1, 26.0, 0.0, screenHeight - 27.0); //bottom
	walls.at(1)->CreateWall(m_pBackBuffer, 26.0, screenHeight-1, 0.0, 0.0); //left
	walls.at(2)->CreateWall(m_pBackBuffer, 26.0, screenHeight - 1, screenWidth - 27.0, 0.0); //Right
	walls.at(3)->CreateWall(m_pBackBuffer, screenWidth - 1, 26.0, 0.0, 0.0); //Top
	walls.at(4)->CreateWall(m_pBackBuffer, screenWidth/2, 26.0, 0.0, 600); //Middle
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
