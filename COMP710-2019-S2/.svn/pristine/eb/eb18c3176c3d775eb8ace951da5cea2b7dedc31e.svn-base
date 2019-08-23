// COMP710 GP 2D Framework 2019

// This include:
#include "inputhandler.h"

// Local includes:
#include "game.h"
#include "logmanager.h"

// Library includes:
#include <cassert>

InputHandler::InputHandler()
: m_pGameController(0)
{

}

InputHandler::~InputHandler()
{
	if (m_pGameController)
	{
		SDL_JoystickClose(m_pGameController);
		m_pGameController = 0;
	}
}

bool 
InputHandler::Initialise()
{
	int numControllesr = SDL_NumJoysticks();

	m_pGameController = SDL_JoystickOpen(0);

	if (!m_pGameController)
	{
		LogManager::GetInstance().Log("No controller detected!");
	}

	return (true);
}

void 
InputHandler::ProcessInput(Game& game)
{
	// SS04.3: Receive Input Events below...

	SDL_Event event;

	while (SDL_PollEvent(&event) != 0)
	{
		if (event.type == SDL_QUIT)
		{
			game.Quit();
		}
		else if (event.type == SDL_KEYDOWN)
		{
			if (event.key.keysym.sym == SDLK_ESCAPE)
			{
				game.Quit();
			}
			if (event.key.keysym.sym == SDLK_d) {
				game.m_pSceneManager->inGameScene->SetMoveRight(true);
			}
			if (event.key.keysym.sym == SDLK_a) {
				game.m_pSceneManager->inGameScene->SetMoveLeft(true);
			}
			if (event.key.keysym.sym == SDLK_e) {
				game.m_pSceneManager->inGameScene->SetShoot(true);
			}
		}
		else if (event.type = SDL_KEYUP) {
			if (event.key.keysym.sym == SDLK_d) {
				game.m_pSceneManager->inGameScene->SetMoveRight(false);
			}
			if (event.key.keysym.sym == SDLK_a) {
				game.m_pSceneManager->inGameScene->SetMoveLeft(false);
			}
			if (event.key.keysym.sym == SDLK_e) {
				game.m_pSceneManager->inGameScene->SetShoot(false);
			}
		}
		// SS04.6: Tell the game to fire a player bullet...
		else if (event.jbutton.button == 10)
		{
			//game.FireSpaceShipBullet();
		}

		// SS04.3: Tell the game to move the space ship left...

		// SS04.3: Tell the game to move the space ship right...
	}
}
