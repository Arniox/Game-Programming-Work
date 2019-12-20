//Locals
#include "InstructionMenu.h"
#include "backbuffer.h"
#include "FontManager.h"
#include "game.h"
#include "logmanager.h"
#include "GUI.h"
#include "SceneManager.h"

InstructionMenu::InstructionMenu()
{
	LogManager::Title("Instructions Scene");

	BackBuffer* backBuffer = Game::GetBackBuffer();
	backBuffer->SetClearColour(51, 51, 51);
	FontManager* fontManager = Game::GetFontManager();

	int screenWidth = Game::GetInstance()->GetScreenWidth();
	int screenHeight = Game::GetInstance()->GetScreenHeight();
	int halfSWidth = screenWidth / 2;

	m_pGUI = new GUI();
	m_pGUI->AddButton("BACK", halfSWidth, static_cast<int>(screenHeight * 0.75), [this](void) {Game::GetSceneManager()->SetNextState(SCENE_MAIN_MENU); });
	m_pGUI->AddButton("EXIT", halfSWidth, static_cast<int>(screenHeight * 0.90), [this](void) {Game::GetInstance()->Quit(); });
}
InstructionMenu::~InstructionMenu()
{
	delete m_pGUI;
	m_pGUI = nullptr;
}

void 
InstructionMenu::Proccess(float deltaTime)
{
	m_pGUI->Process(deltaTime);
}

void 
InstructionMenu::Draw(BackBuffer & backBuffer)
{
	m_pGUI->Draw(backBuffer);
}

void 
InstructionMenu::ProcessControls()
{
	if (m_pInput->KeyDown(KEY_W) || m_pInput->KeyDown(KEY_UP) || m_pInput->JoyHatDown(HAT_UP))
		m_pGUI->IncreaseSelectedElement();
	if (m_pInput->KeyDown(KEY_S) || m_pInput->KeyDown(KEY_DOWN) || m_pInput->JoyHatDown(HAT_DOWN))
		m_pGUI->DecreaseSelectedElement();
	if (m_pInput->KeyDown(KEY_RETURN) || m_pInput->KeyDown(KEY_E) || m_pInput->JoyButtonDown(JOY_A))
		m_pGUI->Click();
}
