#pragma once
#include "Walls.h"
#include "backbuffer.h"
#include <vector>

class WallController
{
public:
	WallController();
	~WallController();

	void Initialise(BackBuffer* m_pBackBuffer, int screenWidth, int screenHeight);
	void Process(float deltaTime);
	void Draw(BackBuffer& m_pBackBuffer);

	std::vector<Walls*> GetWalls();

private:
	std::vector<Walls*> walls;
};

