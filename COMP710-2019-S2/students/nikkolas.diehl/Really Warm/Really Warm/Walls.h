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
	void UpdateWall(float x1, float y1, float x2, float y2);
	void Draw(BackBuffer& backBuffer);
	void CreateWall(BackBuffer* backBuffer, float x1, float y1, float x2, float y2);

	float* GetPoint1();
	float* GetPoint2();

	//Physics calculations
	float GetWallAngle();
	int GetDxl();
	int GetDyl();

protected:
	void SetCenter(float x, float y);

private:
	//Positions
	float mx_arr_point1[2];
	float mx_arr_point2[2];
	//Rotation
	float mx_f_wallAngleDeg;
	float mx_f_wallAngleRad;
	//Imaging
	float mx_f_imageCenterX;
	float mx_f_imageCenterY;
	int mx_i_wallLength;
	//Position
	float xPos;
	float yPos;
};

