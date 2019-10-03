#ifndef __INPUTHANDER_H__
#define __INPUTHANDER_H__

//Locals
#include "Vector2.h"

// Library includes:
#include <SDL.h>
#include <map>
#include <string>

// Forward Declarations:
class Game;

class InputHandler
{
	//Member Methods:
public:
	InputHandler();
	~InputHandler();

	bool Initialise();
	void ProcessInput(Game& game);
	void ExitGame();

protected:
	SDL_Joystick* m_pGameController;
	Game* gameReference;

	//Keyboard map
	std::map<std::string, bool> mx_map_keyBoardLayout;
	std::map<std::string, bool>::iterator mx_iter_mapIterator;

private:
	InputHandler(const InputHandler& inputHandler);
	InputHandler& operator=(const InputHandler& inputHandler);

	//Member Data:
	void updateKeyBoardLayout(std::string in, bool keyPressed);
	bool checkExtistingKey(std::string in);

public:
	//keyboard layout
	bool GetKeyBoardLayout(std::string key);
	//Mouse state
	std::unique_ptr<Vector2> GetMousePos();
	bool* GetMouseClickStates();

private:
	enum JOY_BUTTONS {
		J_A,
		J_B,
		J_X,
		J_Y,
		J_LB,
		J_RB,
		J_SELECT,
		J_START,
		J_LEFT_THUMB,
		J_RIGHT_THUMB
	};

	enum JOY_PAD {
		P_CENTERED,
		P_UP,
		P_RIGHT,
		P_UP_RIGHT,
		P_DOWN,
		P_UNUSED_1,
		P_DOWN_RIGHT,
		P_UNUSED_2,
		P_LEFT,
		P_LEFT_UP,
		P_UNUSED_3,
		P_UNUSED_4,
		P_DOWN_LEFT
	};
};

#endif // __INPUTHANDER_H__
