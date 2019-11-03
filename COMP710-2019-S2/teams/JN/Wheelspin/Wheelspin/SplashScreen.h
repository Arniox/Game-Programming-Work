#ifndef __SPLASH_SCREEN_H__
#define __SPLASH_SCREEN_H__

#include "State.h"

class IniParser;
class Sprite;

class SplashScreen :
	public State
{
public:
	SplashScreen(const char* logo, SCENES nextScene);
	~SplashScreen();
	void Proccess(float deltaTime);
	void Draw(BackBuffer & backBuffer);
	void ProcessControls();
	// Member Variables
protected:
	float m_pScreenTime;
	float m_pMaxSplashTime;
	Sprite* m_pLogo;
	SCENES m_nextScene;
};

#endif // !__SPLASH_SCREEN_H__