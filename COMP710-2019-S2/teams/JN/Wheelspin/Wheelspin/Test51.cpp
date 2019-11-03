#include "Test51.h"

#include "backbuffer.h"
#include "InputManager.h"
#include "game.h"
#include "logmanager.h"
#include "InputManager.h"
#include "LevelsManager.h"

#include <Box2D/Box2D.h>

Test51::Test51()
: m_pBackBuffer(nullptr)
, m_pInput(nullptr)
, m_pLevelsManager()
{

	//Levels testing
	//m_pLevelsManager = new LevelsManager(m_pWorld);
	m_pLevelsManager->SetLevel(1);

	m_pBackBuffer = Game::GetBackBuffer();
	m_width = Game::GetInstance()->GetScreenWidth();
	m_height = Game::GetInstance()->GetScreenHeight();

	m_pInput = Game::GetInputManager();
}

Test51::~Test51()
{
	delete m_pLevelsManager;
	m_pLevelsManager = nullptr;
}

void
Test51::Proccess(float deltaTime)
{
	
}

void
Test51::Draw(BackBuffer& backBuffer)
{
	m_pLevelsManager->DrawLevel(backBuffer);
}

void
Test51::ProcessControls()
{	
	if (m_pInput->KeyDown(KEY_ESCAPE))
		Game::GetInstance()->Quit();
}