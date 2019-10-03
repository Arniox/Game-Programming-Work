#define _CRTDBG_MAP_ALLOC
// COMP710 GP 2D Framework 2019

// This include:
#include "inputhandler.h"

// Local includes:
#include "game.h"
#include "logmanager.h"

// Library includes:
#include <cassert>
#include <map>

//Mouse data
struct MouseInfo {
	int x = 0;
	int y = 0;
	bool LEFT_CLICK = false;
	bool MIDDLE_CLICK = false;
	bool RIGHT_CLICK = false;
};

MouseInfo* mouse;

InputHandler::InputHandler()
: m_pGameController(0)
, mx_map_keyBoardLayout()
, mx_iter_mapIterator()
{	
}

InputHandler::~InputHandler()
{
	mx_map_keyBoardLayout.clear();
	delete mouse;
	mouse = nullptr;

	if (m_pGameController)
	{
		m_pGameController = nullptr;
	}
}

bool 
InputHandler::Initialise()
{
	int numControllesr = SDL_NumJoysticks();

	m_pGameController = SDL_JoystickOpen(0);

	mouse = new MouseInfo();

	if (!m_pGameController)
	{
		LogManager::GetInstance().Log("No controller detected!");
		LogManager::GetInstance().DestroyInstance();
	}
	else {
		LogManager::GetInstance().Log("Controller Detected!");
		LogManager::GetInstance().DestroyInstance();
	}

	return (true);
}

void 
InputHandler::ProcessInput(Game& game)
{
	SDL_Event event;
	gameReference = &game;

	//Mouse events
	mouse->LEFT_CLICK = false;
	mouse->RIGHT_CLICK = false;
	mouse->MIDDLE_CLICK = false;

	SDL_GetMouseState(&mouse->x, &mouse->y);
	if (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_LEFT)) {
		mouse->LEFT_CLICK = true;
	}
	else if (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_RIGHT)) {
		mouse->RIGHT_CLICK = true;
	}
	else if (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_MIDDLE)) {
		mouse->MIDDLE_CLICK = true;
	}

	//Keyboard and controller events
	while (SDL_PollEvent(&event))
	{
		switch (event.type) {
			case SDL_QUIT:
				game.Quit();
				break;
			case SDL_KEYDOWN:
				if (event.key.keysym.sym == SDLK_ESCAPE)
				{
					updateKeyBoardLayout("escape", true);
				}
				if (event.key.keysym.sym == SDLK_d) {
					updateKeyBoardLayout("d", true);
				}
				if (event.key.keysym.sym == SDLK_a) {
					updateKeyBoardLayout("a", true);
				}
				if (event.key.keysym.sym == SDLK_w) {
					updateKeyBoardLayout("w", true);
				}
				if (event.key.keysym.sym == SDLK_s) {
					updateKeyBoardLayout("s", true);
				}
				if (event.key.keysym.sym == SDLK_SPACE) {
					updateKeyBoardLayout("space", true);
				}
				if (event.key.keysym.sym == SDLK_q) {
					updateKeyBoardLayout("q", true);
				}
				if (event.key.keysym.sym == SDLK_UP) {
					updateKeyBoardLayout("up", true);
				}
				if (event.key.keysym.sym == SDLK_DOWN) {
					updateKeyBoardLayout("down", true);
				}
				if (event.key.keysym.sym == SDLK_RETURN) {
					updateKeyBoardLayout("enter", true);
				}
				break;
			case SDL_KEYUP:
				if (event.key.keysym.sym == SDLK_ESCAPE)
				{
					updateKeyBoardLayout("escape", false);
				}
				if (event.key.keysym.sym == SDLK_d) {
					updateKeyBoardLayout("d", false);
				}
				if (event.key.keysym.sym == SDLK_a) {
					updateKeyBoardLayout("a", false);
				}
				if (event.key.keysym.sym == SDLK_w) {
					updateKeyBoardLayout("w", false);
				}
				if (event.key.keysym.sym == SDLK_s) {
					updateKeyBoardLayout("s", false);
				}
				if (event.key.keysym.sym == SDLK_SPACE) {
					updateKeyBoardLayout("space", false);
				}
				if (event.key.keysym.sym == SDLK_q) {
					updateKeyBoardLayout("q", false);
				}
				if (event.key.keysym.sym == SDLK_UP) {
					updateKeyBoardLayout("up", false);
				}
				if (event.key.keysym.sym == SDLK_DOWN) {
					updateKeyBoardLayout("down", false);
				}
				if (event.key.keysym.sym == SDLK_RETURN) {
					updateKeyBoardLayout("enter", false);
				}
				break;
			case SDL_JOYBUTTONDOWN:
				//Simulate keyboard events
				if (event.jbutton.button == J_A) {
					updateKeyBoardLayout("space", true);
				}
				if (event.jbutton.button == J_Y) {
					updateKeyBoardLayout("q", true);
				}
				if (event.jbutton.button == J_B) {
					updateKeyBoardLayout("enter", true);
				}
				if (event.jbutton.button == J_START) {
					updateKeyBoardLayout("escape", true);
				}
				break;
			case SDL_JOYBUTTONUP:
				if (event.jbutton.button == J_A) {
					updateKeyBoardLayout("space", false);
				}
				if (event.jbutton.button == J_Y) {
					updateKeyBoardLayout("q", false);
				}
				if (event.jbutton.button == J_B) {
					updateKeyBoardLayout("enter", false);
				}
				if (event.jbutton.button == J_START) {
					updateKeyBoardLayout("escape", false);
				}
				break;
			case SDL_JOYHATMOTION:
				if (event.jhat.value == P_CENTERED) {
					updateKeyBoardLayout("w", false);
					updateKeyBoardLayout("s", false);
					updateKeyBoardLayout("up", false);
					updateKeyBoardLayout("down", false);
				}
				if (event.jhat.value == P_UP) {
					updateKeyBoardLayout("w", true);
				}
				if (event.jhat.value == P_DOWN) {
					updateKeyBoardLayout("s", true);
				}
				break;
			case SDL_JOYAXISMOTION:
				if ((event.jaxis.value < -3200) || (event.jaxis.value > 3200)) {
					if (event.jaxis.axis == 0) {
						//left side movement
						updateKeyBoardLayout("d", false);
						updateKeyBoardLayout("a", false);

						if (event.jaxis.value > 0) {
							updateKeyBoardLayout("d", true);
						}
						else {
							updateKeyBoardLayout("a", true);
						}
					}

					//Other joystick
					if (event.jaxis.axis == 3) {
						if (event.jaxis.value > 0) {
							mouse->x = 3000;
							mouse->y = 500;
						}
						else {
							mouse->x = -3000;
							mouse->y = 500;
						}
					}
					if (event.jaxis.axis == 4) {
						if (event.jaxis.value > 0) {
							mouse->x = 500;
							mouse->y = 3000;
						}
						else {
							mouse->x = 500;
							mouse->y = -3000;
						}
					}
				}
				break;
			default:
				break;
		}

		/*
		if (event.type == SDL_QUIT)
		{
			game.Quit();
		}
		else if (event.type == SDL_KEYDOWN)
		{
			if (event.key.keysym.sym == SDLK_ESCAPE)
			{
				updateKeyBoardLayout("escape", true);
			}
			if (event.key.keysym.sym == SDLK_d) {
				updateKeyBoardLayout("d", true);
			}
			if (event.key.keysym.sym == SDLK_a) {
				updateKeyBoardLayout("a", true);
			}
			if (event.key.keysym.sym == SDLK_w) {
				updateKeyBoardLayout("w", true);
			}
			if (event.key.keysym.sym == SDLK_s) {
				updateKeyBoardLayout("s", true);
			}
			if (event.key.keysym.sym == SDLK_SPACE) {
				updateKeyBoardLayout("space", true);
			}
			if (event.key.keysym.sym == SDLK_q) {
				updateKeyBoardLayout("q", true);
			}
			if (event.key.keysym.sym == SDLK_UP) {
				updateKeyBoardLayout("up", true);
			}
			if (event.key.keysym.sym == SDLK_DOWN) {
				updateKeyBoardLayout("down", true);
			}
			if (event.key.keysym.sym == SDLK_RETURN) {
				updateKeyBoardLayout("enter", true);
			}
		}
		else if (event.type = SDL_KEYUP) {
			if (event.key.keysym.sym == SDLK_ESCAPE)
			{
				updateKeyBoardLayout("escape", false);
			}
			if (event.key.keysym.sym == SDLK_d) {
				updateKeyBoardLayout("d", false);
			}
			if (event.key.keysym.sym == SDLK_a) {
				updateKeyBoardLayout("a", false);
			}
			if (event.key.keysym.sym == SDLK_w) {
				updateKeyBoardLayout("w", false);
			}
			if (event.key.keysym.sym == SDLK_s) {
				updateKeyBoardLayout("s", false);
			}
			if (event.key.keysym.sym == SDLK_SPACE) {
				updateKeyBoardLayout("space", false);
			}
			if (event.key.keysym.sym == SDLK_q) {
				updateKeyBoardLayout("q", false);
			}
			if (event.key.keysym.sym == SDLK_UP) {
				updateKeyBoardLayout("up", false);
			}
			if (event.key.keysym.sym == SDLK_DOWN) {
				updateKeyBoardLayout("down", false);
			}
			if (event.key.keysym.sym == SDLK_RETURN) {
				updateKeyBoardLayout("enter", false);
			}
		}
		else if (event.type == SDL_JOYBUTTONDOWN)
		{
			if (event.jbutton.button == 3) {
				double i = 0;
			}
		}
		else if (event.type == SDL_JOYBUTTONUP)
		{
			if (event.jbutton.button == 3) {
				double i = 0;
			}
		}*/
	}
}

void
InputHandler::ExitGame() 
{
	gameReference->Quit();
}

void
InputHandler::updateKeyBoardLayout(std::string in, bool keyPressed) {
	//Assign key into map
	mx_map_keyBoardLayout[in] = keyPressed;
}

bool
InputHandler::checkExtistingKey(std::string in) {
	mx_iter_mapIterator = mx_map_keyBoardLayout.find(in);

	//Found value
	if (mx_iter_mapIterator != mx_map_keyBoardLayout.end()) {
		return true;
	}
	else {
		return false;
	}
}

bool
InputHandler::GetKeyBoardLayout(std::string key)
{
	if (checkExtistingKey(key)) {
		return mx_map_keyBoardLayout[key];
	}
	else {
		return false;
	}
}

std::unique_ptr<Vector2>
InputHandler::GetMousePos()
{
	std::unique_ptr<Vector2> smart_mousePos = std::make_unique<Vector2>();

	smart_mousePos->x = mouse->x;
	smart_mousePos->y = mouse->y;

	return smart_mousePos;
}

bool*
InputHandler::GetMouseClickStates()
{
	static bool states[3];
	states[0] = mouse->LEFT_CLICK;
	states[2] = mouse->MIDDLE_CLICK;
	states[1] = mouse->RIGHT_CLICK;

	return states;
}
