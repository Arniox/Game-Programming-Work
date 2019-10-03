// COMP710 GP 2D Framework 2019
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#ifdef _DEBUG
	#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
	#define new DEBUG_NEW
#endif
// Library includes:
#include <SDL.h>

// Local includes:
#include "game.h"
#include "logmanager.h"


int main(int argc, char* argv[])
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	Game& gameInstance = Game::GetInstance();
	if (!gameInstance.Initialise())
	{
		LogManager::GetInstance().Log("Game initialise failed!");

		return (1);
	}
	
	while (gameInstance.DoGameLoop())
	{
		// No body.
	}

	Game::DestroyInstance();
	LogManager::GetInstance().DestroyInstance();
	
	//_CrtDumpMemoryLeaks();

	return (0);
}
