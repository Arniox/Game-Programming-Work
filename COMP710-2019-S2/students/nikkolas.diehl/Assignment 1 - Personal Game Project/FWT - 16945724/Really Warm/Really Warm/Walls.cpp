#include "Walls.h"

Walls::Walls()
	:mx_arr_point1()
	,mx_arr_point2()
	,mx_f_wallAngleRad(0)
	,mx_f_wallAngleDeg(0)
	,mx_f_imageCenterX(0)
	,mx_f_imageCenterY(0)
{
}
Walls::~Walls()
{
}

void 
Walls::UpdateWall(float x1, float y1, float x2, float y2)
{	
	//Set wall points
	mx_arr_point1[0] = x1;
	mx_arr_point1[1] = y1;
	mx_arr_point2[0] = x2;
	mx_arr_point2[1] = y2;

	//Get angle of wall for sprite rotation
	mx_f_wallAngleRad = (float)(atan2(mx_arr_point2[1] - mx_arr_point1[1], mx_arr_point2[0] - mx_arr_point1[0]));
	mx_f_wallAngleDeg = (float)(mx_f_wallAngleRad * 180 / M_PI);

	//Get center point of wall
	mx_f_imageCenterX = (float)((mx_arr_point1[0] + mx_arr_point2[0]) / 2);
	mx_f_imageCenterY = (float)((mx_arr_point1[1] + mx_arr_point2[1]) / 2);

	//Get wall length
	mx_i_wallLength = (int)(abs(mx_arr_point2[0] - mx_arr_point1[0]));

	SetCenter(x1, y1);
}

void 
Walls::CreateWall(BackBuffer* m_pBackBuffer, float x1, float y1, float x2, float y2) {
	Entity::Initialise(m_pBackBuffer->CreateSprite("assets/Sprites/WallSegment-Small.png"));
	UpdateWall(x1, y1, x2, y2);
}


void 
Walls::Draw(BackBuffer & backBuffer)
{
	assert(m_pSprite);
	//Set angle and then go through normal drawing process
	m_pSprite->SetAngle(mx_f_wallAngleDeg);
	m_pSprite->SetLength(mx_i_wallLength);
	Entity::Draw(backBuffer);
}

float*
Walls::GetPoint1()
{
	return mx_arr_point1;
}

float* 
Walls::GetPoint2()
{
	return mx_arr_point2;
}

void
Walls::SetCenter(float x, float y)
{
	m_x = x;
	m_y = y;
}

float
Walls::GetWallAngle()
{
	return mx_f_wallAngleDeg;
}

int
Walls::GetDxl()
{
	return (int)(mx_arr_point2[0] - mx_arr_point1[0]);
}

int
Walls::GetDyl() 
{
	return (int)(mx_arr_point2[1] - mx_arr_point1[1]);
}




