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
Walls::CreateWall(BackBuffer* backBuffer, double wallWidth, double wallHeight, double x, double y) 
{
	entityBox = new BoundingBox(0, 0, 0, 0);

	Initialise(backBuffer->CreateSprite("assets/Sprites/WallSegment-Small.png"));
	SetSize(wallWidth, wallHeight);
	SetCenter(x, y);

	mx_d_wallAngleDeg = (wallWidth > wallHeight ? 0 : 90);

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

	//Shift over to fix rotated walls
	int X, Y, W, H;
	X = static_cast<int>(entityBox->X() - 1);
	Y = static_cast<int>(entityBox->Y() - 1);
	W = static_cast<int>(entityBox->W() + 1);
	H = static_cast<int>(entityBox->H() + 1);

	/*
	backBuffer.DrawLine(X, Y, X + W, Y);
	backBuffer.DrawLine(X, Y, X, Y + H);
	backBuffer.DrawLine(X + W, Y, X + W, Y + H);
	backBuffer.DrawLine(X, Y + H, X + W, Y + H);
	*/


	assert(mo_sprite);
	//Set angle and then go through normal drawing process
	mo_sprite->SetAngle(mx_d_wallAngleDeg);
	mo_sprite->SetLength((mx_d_wallAngleDeg == 0 ? static_cast<int>(entityBox->W()) : static_cast<int>(entityBox->H())));

	//Set custom X and Y calues as well as rotation point for back buffer and then directly draw. Don't use Entity draw method
	mo_sprite->SetX((mx_d_wallAngleDeg == 0 ? static_cast<int>(entityBox->X()) : static_cast<int>(entityBox->X() + entityBox->W())));
	mo_sprite->SetY(static_cast<int>(entityBox->Y()));
	mo_sprite->RotateNull(false);

	mo_sprite->Draw(backBuffer);
}




