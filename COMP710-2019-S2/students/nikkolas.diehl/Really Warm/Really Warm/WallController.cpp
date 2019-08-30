#include "WallController.h"

WallController::WallController()
{
}
WallController::~WallController()
{
	walls.empty();
}

void
WallController::Initialise(BackBuffer* m_pBackBuffer, int screenWidth, int screenHeight) {
	//LEVEL SCANNING FOR WALLS - STUB
	for (int i = 0; i < 2; ++i) {
		walls.push_back(new Walls());
	}

	//Manually set up walls for now
	//backbuffer, angle, length, x, y
	walls.at(0)->CreateWall(m_pBackBuffer, 0, screenWidth - 20, 10, 0); //top
	walls.at(1)->CreateWall(m_pBackBuffer, 0, screenWidth - 20, 10, screenHeight - 20); //bottom
	//walls.at(2)->CreateWall(m_pBackBuffer, 90, screenHeight - 10, 10, 0); //Right
	//walls.at(3)->CreateWall(m_pBackBuffer, 90, screenHeight - 10, screenWidth - 10, 20); //Left
}

void
WallController::Process(float deltaTime) {
	for (int i = 0; i < static_cast<signed int>(walls.size()); ++i) {
		walls.at(i)->Process(deltaTime);
	}
}

void
WallController::Draw(BackBuffer& m_pBackBuffer) {
	for (int i = 0; i < static_cast<signed int>(walls.size()); ++i) {
		walls.at(i)->Draw(m_pBackBuffer);
	}
}

std::vector<Walls*>
WallController::GetWalls() {
	return walls;
}
