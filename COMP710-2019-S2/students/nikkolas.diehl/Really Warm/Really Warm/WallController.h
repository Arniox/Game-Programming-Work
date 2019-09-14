#pragma once

#include "Walls.h"
#include "backbuffer.h"
#include <vector>

class WallController
{
public:
	WallController();
	~WallController();

	void Initialise(BackBuffer* m_pBackBuffer, double screenWidth, double screenHeight);
	void Draw(BackBuffer& m_pBackBuffer);

	std::vector<Walls*>* GetWalls();

private:
	std::vector<Walls*> walls;
};

