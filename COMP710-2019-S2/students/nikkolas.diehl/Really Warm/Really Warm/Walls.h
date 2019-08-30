#pragma once

#include "backbuffer.h"
#include "entity.h"
#include "Article.h"

// Library includes:
#include <cassert>
#include <cmath>

class Walls:
	public Article
{
public:
	Walls();
	~Walls();

	//Wall Creation
	void UpdateWall(int wallLength, float wallAngle, int x1, int y1);
	void Draw(BackBuffer& backBuffer);
	void CreateWall(BackBuffer* backBuffer, float wallAngle, int wallLength, int x1, int y1);

	int* GetPoint1();
	int* GetPoint2();

	//Physics calculations
	float GetWallAngle();
	int GetDxl();
	int GetDyl();

protected:
	void SetCenter(int x, int y);

private:
	//Positions
	int mx_arr_point1[2];
	int mx_arr_point2[2];
	//Rotation
	float mx_f_wallAngleDeg;
	float mx_f_wallAngleRad;
	//Imaging
	int mx_i_imageCenterX;
	int mx_i_imageCenterY;
	int mx_i_wallLength;
};

