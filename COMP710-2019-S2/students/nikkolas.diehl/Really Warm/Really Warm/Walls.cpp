#include "Walls.h"

Walls::Walls()
:mx_d_wallAngleDeg(0.0)
{
}
Walls::~Walls()
{
	delete entityBox;
	entityBox = 0;
}

void 
Walls::UpdateWall(double wallAngle)
{	
	//Get angle of wall for sprite rotation
	mx_d_wallAngleDeg = wallAngle;
	float wallAngleRad = static_cast<float>(mx_d_wallAngleDeg * M_PI/180.0f);
}

void 
Walls::CreateWall(BackBuffer* backBuffer, double wallAngle, double wallWidth, double wallHeight, double x, double y) 
{
	entityBox = new Rectangle(0, 0, 0, 0);

	Initialise(backBuffer->CreateSprite("assets/Sprites/WallSegment-Small.png"));
	SetSize(wallWidth, wallHeight);
	SetCenter(x, y);

	UpdateWall(wallAngle);

	mat->isKinematic = false;
	mat->movementSpeedLimit = 0;
	mat->mass = INFINITY;
	mat->restitution = 0.0001;

	//Set up
	linearVelocity = new Vector2();
	entityAngle = degreesToRadians(0);
	anglularVelocity = 0;
	force = new Vector2();
}


void 
Walls::Draw(BackBuffer & backBuffer)
{
	backBuffer.SetDrawColour(255, 255, 0);

	int X = static_cast<int>(entityBox->X());
	int Y = static_cast<int>(entityBox->Y());
	int W = static_cast<int>(entityBox->W());
	int H = static_cast<int>(entityBox->H());

	backBuffer.DrawLine(X, Y, X + W, Y);
	backBuffer.DrawLine(X, Y, X, Y + H);
	backBuffer.DrawLine(X + W, Y, X + W, Y + H);
	backBuffer.DrawLine(X, Y + H, X + W, Y + H);


	//assert(m_pSprite);
	//Set angle and then go through normal drawing process
	//m_pSprite->SetAngle(mx_f_wallAngleDeg);
	//m_pSprite->SetLength((mx_f_wallAngleDeg == 0 ? mo_i_Width : mo_i_Height));
	//Entity::Draw(backBuffer);
}




