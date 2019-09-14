#pragma once
#include "entity.h"

class SplashScreenImage :
	public Entity
{
public:
	SplashScreenImage();
	~SplashScreenImage();

	void CreateImage(BackBuffer* backBuffer, double x, double y);
};

