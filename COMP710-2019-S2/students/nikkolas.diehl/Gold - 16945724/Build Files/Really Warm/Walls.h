#ifndef __WALLS_H__
#define __WALLS_H__

//Locals
#include "iniParser.h"
#include "entity.h"
#include "backbuffer.h"
#include "Vector2.h"

// Library includes:
#include <cassert>
#include <cmath>

class Walls:
	public Entity
{
public:
	Walls();
	~Walls();

	//Wall Creation
	void CreateWall(IniParser* iniparser, BackBuffer* backBuffer, double wallWidth, double wallHeight, double x, double y);
	void Draw(BackBuffer& backBuffer);
};

#endif
