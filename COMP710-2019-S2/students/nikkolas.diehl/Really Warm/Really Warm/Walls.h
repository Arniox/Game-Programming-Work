#pragma once

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
	void CreateWall(BackBuffer* backBuffer, double wallAngle, double wallWidth, double wallHeight, double x, double y);
	void UpdateWall(double wallAngle);
	void Draw(BackBuffer& backBuffer);

private:
	//Rotation
	double mx_d_wallAngleDeg;
};

