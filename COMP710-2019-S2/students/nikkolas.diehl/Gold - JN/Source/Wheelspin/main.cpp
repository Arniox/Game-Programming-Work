// COMP710 GP 2D Framework 2019

// Library includes:
#include <crtdbg.h>
#include <SDL.h>
#include <Box2D/Box2D.h>
#include <string>

// Local includes:
#include "game.h"
#include "logmanager.h"

#include "AnimatedUI.h"

int main(int argc, char* argv[])
{
	// Enable memory-leak checking for entire project
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	Game* gameInstance = Game::GetInstance();

	if (!gameInstance->Initialise())
	{
		LogManager::Debug("Game initialise failed!");

		return (1);
	}
	
	while (gameInstance->DoGameLoop())
	{
		// No body.
	}

	Game::DestroyInstance();
	LogManager::GetInstance().DestroyInstance();

	return (0);
}