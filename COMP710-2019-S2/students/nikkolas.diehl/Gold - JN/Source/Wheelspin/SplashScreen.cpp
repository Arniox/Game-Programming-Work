#include "SplashScreen.h"
#include "backbuffer.h"
#include "IniParser.h"
#include "InputManager.h"
#include "sprite.h"
#include "SceneManager.h"
#include "game.h"

#include "logmanager.h"
#include <string>

SplashScreen::SplashScreen(const char* logo, SCENES nextScene)
: m_pMaxSplashTime(0.0f)
, m_pScreenTime(0.0f)
, m_nextScene(SCENE_QUIT)
{
	LogManager::Title("Splash Screen");

	Game* game = Game::GetInstance();
	IniParser* iniFile = game->GetIniFile();
	BackBuffer* backBuffer = game->GetBackBuffer();
	m_pInput = game->GetInputManager();
	m_pMaxSplashTime = iniFile->GetValueAsFloat("SplashScreen", "time");
	m_nextScene = nextScene;
	m_pLogo = backBuffer->CreateSprite(iniFile->GetValueAsString("SplashScreen", logo).c_str());
	m_pLogo->SetX((game->GetScreenWidth() / 2) - (m_pLogo->GetWidth() / 2));
	m_pLogo->SetY((game->GetScreenHeight() / 2 ) - (m_pLogo->GetHeight() / 2));
	backBuffer->SetClearColour(0x00, 0x00, 0x00);
}


SplashScreen::~SplashScreen()
{
	delete m_pLogo;
	m_pLogo = nullptr;
}

void
SplashScreen::Proccess(float deltaTime)
{
	m_pScreenTime += deltaTime;
	if (m_pScreenTime >= m_pMaxSplashTime)
	{
		// Splash Screen State is over
		Game::GetSceneManager()->SetNextState(m_nextScene);
	}
}

void
SplashScreen::Draw(BackBuffer& backBuffer)
{
	m_pLogo->Draw(backBuffer);
}

void
SplashScreen::ProcessControls()
{
	// Skip current screen
	if (m_pInput->KeyDown(KEY_SPACE) || m_pInput->JoyButtonDown(JOY_A))
		Game::GetSceneManager()->SetNextState(m_nextScene);
	// ESC, quit game
	if (m_pInput->KeyDown(KEY_ESCAPE) || m_pInput->JoyButtonDown(JOY_START))
		Game::GetInstance()->Quit();
}