#define _CRTDBG_MAP_ALLOC
#include "SplashScreen.h"

SplashScreen::SplashScreen()
: splashImg(0)
{
}
SplashScreen::~SplashScreen()
{
	delete splashImg;
	splashImg = nullptr;
}

void
SplashScreen::Initialise(BackBuffer* backBuffer, int screenWidth, int screenHeight)
{
	splashImg = new UIElement();
	splashImg->CreateUIElement("assets/Images/autLogo.png", backBuffer, "", screenWidth/2, 0, 1, 0);
}

void
SplashScreen::Draw(BackBuffer& backBuffer)
{
	backBuffer.SetClearColour(0, 0, 0);
	splashImg->DrawUIElement(backBuffer, 255, false);
}