#pragma once
#include "Rectangle.h"
#include "inputhandler.h"
#include "entity.h"
#include "backbuffer.h"
#include "SplashScreenImage.h"

class SplashScreen
{
public:
	SplashScreen();
	~SplashScreen();

	void Initialise(BackBuffer* backBuffer, int screenWidth, int screenHeight);
	void Draw(BackBuffer& backBuffer);

protected:
	SplashScreenImage* splash;

};

