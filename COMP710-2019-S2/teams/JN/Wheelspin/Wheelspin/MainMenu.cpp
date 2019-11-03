//Locals
#include "MainMenu.h"
#include "backbuffer.h"
#include "FontManager.h"
#include "game.h"
#include "logmanager.h"
#include "GUI.h"
#include "SceneManager.h"
#include "IniParser.h"

MainMenu::MainMenu()
{	
	LogManager::Title("Main Menu");

	BackBuffer* backBuffer = Game::GetBackBuffer();
	backBuffer->SetClearColour(51, 51, 51);
	FontManager* fontManager = Game::GetFontManager();

	// Repeditive calls should be avoided, store results on the stack to avoid calling multiple times.
	int screenWidth = Game::GetInstance()->GetScreenWidth();
	int screenHeight = Game::GetInstance()->GetScreenHeight();
	int halfSWidth = screenWidth / 2;
	SCENES sceneToGo = static_cast<SCENES>(Game::GetIniFile()->GetValueAsInt("Game", "menuButtonStateLead"));	

	m_pGUI = new GUI();
	m_pGUI->AddButton("PLAY", halfSWidth, static_cast<int>(screenHeight * 0.10), [=](void) {Game::GetSceneManager()->SetNextState(sceneToGo); });
	m_pGUI->AddButton("INSTRUCTIONS", halfSWidth, static_cast<int>(screenHeight * 0.30), [this](void) {Game::GetSceneManager()->SetNextState(SCENE_INSTRUCTIONS); });
	m_pGUI->AddButton("EXIT", halfSWidth, static_cast<int>(screenHeight * 0.90), [this](void) {Game::GetInstance()->Quit();  });
}

MainMenu::~MainMenu()
{
	delete m_pGUI;
	m_pGUI = nullptr;
}

void 
MainMenu::Proccess(float deltaTime)
{
	m_pGUI->Process(deltaTime);
}

void 
MainMenu::Draw(BackBuffer& backBuffer)
{
	m_pGUI->Draw(backBuffer);
}

void 
MainMenu::ProcessControls()
{
	if (m_pInput->KeyDown(KEY_W) || m_pInput->KeyDown(KEY_UP) ||m_pInput->JoyHatDown(HAT_UP))
		m_pGUI->IncreaseSelectedElement();
	if (m_pInput->KeyDown(KEY_S) || m_pInput->KeyDown(KEY_DOWN) || m_pInput->JoyHatDown(HAT_DOWN))
		m_pGUI->DecreaseSelectedElement();
	if (m_pInput->KeyDown(KEY_RETURN) || m_pInput->KeyDown(KEY_E) || m_pInput->JoyButtonDown(JOY_A))
		m_pGUI->Click();
}
