#ifndef __SPLASHSCREEN_H__
#define __SPLASHSCREEN_H__

#include "backbuffer.h"
#include "UIElement.h"

class SplashScreen
{
public:
	SplashScreen();
	~SplashScreen();

	void Initialise(BackBuffer* backBuffer, int screenWidth, int screenHeight);
	void Draw(BackBuffer& backBuffer);

protected:
	UIElement* splashImg;

};

#endif

