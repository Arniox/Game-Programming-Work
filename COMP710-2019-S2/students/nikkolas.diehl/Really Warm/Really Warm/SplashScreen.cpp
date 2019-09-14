#include "SplashScreen.h"

SplashScreen::SplashScreen()
: splash(0)
{
}
SplashScreen::~SplashScreen()
{
	delete splash;
	splash = 0;
}

void
SplashScreen::Initialise(BackBuffer* backBuffer, int screenWidth, int screenHeight)
{
	splash = new SplashScreenImage();
	splash->CreateImage(backBuffer, screenWidth/2, 0);
}

void
SplashScreen::Draw(BackBuffer& backBuffer)
{
	backBuffer.SetClearColour(255, 255, 255);
	splash->Draw(backBuffer);
}