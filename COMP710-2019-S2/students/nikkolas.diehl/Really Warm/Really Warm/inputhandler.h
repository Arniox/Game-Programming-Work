// COMP710 GP 2D Framework 2019
#ifndef __INPUTHANDER_H__
#define __INPUTHANDER_H__

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

protected:

private:
	InputHandler(const InputHandler& inputHandler);
	InputHandler& operator=(const InputHandler& inputHandler);

	//Member Data:
	void updateKeyBoardLayout(std::string in, bool keyPressed);
	bool checkExtistingKey(std::string in);
public:
	//keyboard layout
	bool GetKeyBoardLayout(std::string key);

protected:
	SDL_Joystick* m_pGameController;

	//Keyboard map
	std::map<std::string, bool> mx_map_keyBoardLayout;
	std::map<std::string, bool>::iterator mx_iter_mapIterator;

private:

};

#endif // __INPUTHANDER_H__
