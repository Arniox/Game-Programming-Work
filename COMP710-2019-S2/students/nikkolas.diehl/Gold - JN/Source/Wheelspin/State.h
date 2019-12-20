#ifndef __STATE_H__
#define __STATE_H__

#include "SDL.h"
#include "InputManager.h"

#include <string>

class BackBuffer;

enum SCENES
{
	SCENE_NULL,
	SCENE_SPLASH_AUT,
	SCENE_SPLASH_FMOD,
	SCENE_MAIN_MENU,
	SCENE_INSTRUCTIONS,
	SCENE_LEVEL,
	SCENE_TEST,
	SCENE_TEST2,
	SCENE_CAR_TEST,
	SCENE_CAR_TEST2,
	SCENE_TEST51,
	SCENE_QUIT
};

class State
{
public:
	State();
	virtual ~State();
	virtual void Proccess(float deltaTime) = 0;
	virtual void Draw(BackBuffer& backBuffer) = 0;
	virtual void ProcessControls() = 0;

protected:
	InputManager* m_pInput;
};

#endif // !__STATE_H__