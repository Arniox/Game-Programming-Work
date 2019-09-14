#include "SplashScreenImage.h"

SplashScreenImage::SplashScreenImage()
{
}
SplashScreenImage::~SplashScreenImage()
{
	delete linearVelocity;
	linearVelocity = 0;
	delete force;
	force = 0;
	delete entityBox;
	entityBox = 0;
}

void
SplashScreenImage::CreateImage(BackBuffer* backBuffer, double x, double y)
{
	entityBox = new BoundingBox(0, 0, 0, 0);

	Initialise(backBuffer->CreateSprite("assets/Images/autLogo.png"));
	SetSize(mo_sprite->GetWidth(), mo_sprite->GetHeight());
	SetCenter(x - mo_sprite->GetWidth() / 2, y);
}
