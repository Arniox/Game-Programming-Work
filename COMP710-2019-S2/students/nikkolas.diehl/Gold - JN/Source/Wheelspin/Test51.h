#ifndef __TEST51_H__
#define __TEST51_H__

#include "State.h"
class BackBuffer;
class InputManager;
class LevelsManager;

class Test51 :
	public State
{
public:
	Test51();
	~Test51();
	void Proccess(float deltaTime);
	void Draw(BackBuffer & backBuffer);
	void ProcessControls();

private:
	BackBuffer* m_pBackBuffer;
	InputManager* m_pInput;

	LevelsManager* m_pLevelsManager;

	int m_width;
	int m_height;
};

#endif

