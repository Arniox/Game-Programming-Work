#include "Walls.h"

Walls::Walls()
	:mx_arr_point1()
	,mx_arr_point2()
	,mx_f_wallAngleRad(0.0f)
	,mx_f_wallAngleDeg(0.0f)
	,mx_i_imageCenterX(0)
	,mx_i_imageCenterY(0)
{
}
Walls::~Walls()
{
}

void 
Walls::UpdateWall(int wallLength, float wallAngle, int x1, int y1)
{	
	mp_i_SpriteHeight = m_pSprite->GetHeight();

	//Set wall points
	mx_arr_point1[0] = x1;
	mx_arr_point1[1] = y1;
	//Set wall length
	mx_i_wallLength = wallLength;

	//Get angle of wall for sprite rotation
	mx_f_wallAngleDeg = wallAngle;
	mx_f_wallAngleRad = static_cast<float>(mx_f_wallAngleDeg * M_PI/ 180.0f);

	//Set last two points
	mx_arr_point2[0] = static_cast<int>(mx_i_wallLength * cos(mx_f_wallAngleRad) + mx_arr_point1[0]);
	mx_arr_point2[1] = static_cast<int>(mx_i_wallLength * sin(mx_f_wallAngleRad) + mx_arr_point1[1]);

	//Get center point of wall
	mx_i_imageCenterX = mx_i_wallLength / 2;
	mx_i_imageCenterY = mp_i_SpriteHeight / 2;

	SetCenter(x1, y1);
}

void 
Walls::CreateWall(BackBuffer* m_pBackBuffer, float wallAngle, int wallLength, int x1, int y1) {
	Entity::Initialise(m_pBackBuffer->CreateSprite("assets/Sprites/WallSegment-Small.png"));
	UpdateWall(wallLength, wallAngle, x1, y1);
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

int*
Walls::GetPoint1()
{
	return mx_arr_point1;
}

int*
Walls::GetPoint2()
{
	return mx_arr_point2;
}

void
Walls::SetCenter(int x, int y)
{
	m_x = static_cast<float>(x);
	m_y = static_cast<float>(y);
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




