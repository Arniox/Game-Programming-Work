#include "GUI.h"

//Locals
#include "game.h"
#include "StaticUI.h"
#include "Button.h"
#include "IniParser.h"
#include "Rectangle.h"
#include "InputManager.h"
#include "backbuffer.h"
#include "Vector2.h"
#include "SoundManager.h"
#include "FontManager.h"
#include "logmanager.h"

GUI::GUI()
: m_selected(-1)
, m_previouslySelected(-1)
{
	m_pButtons.clear();
	m_pButtons.shrink_to_fit();

	m_pInput = Game::GetInputManager();

	M2P = 0;
	P2M = 0;
}

GUI::GUI(float _M2P, float _P2M)
{
	m_pButtons.clear();
	m_pButtons.shrink_to_fit();

	m_pInput = Game::GetInputManager();

	M2P = _M2P;
	P2M = _P2M;
}
GUI::~GUI()
{
	for (std::vector<Button*>::iterator it = m_pButtons.begin(); it != m_pButtons.end(); ++it) {
		delete (*it);
		(*it) = nullptr;
	}
	for (std::vector<HudItem*>::iterator it = m_pHudElements.begin(); it != m_pHudElements.end(); ++it) {
		delete (*it);
		(*it) = nullptr;
	}

	m_pButtons.clear();
	m_pButtons.shrink_to_fit();

	m_pHudElements.clear();
	m_pHudElements.shrink_to_fit();

	m_pInput = nullptr;
}

void 
GUI::AddButton(std::string ButtonText, int x, int y, CallBack callBack)
{
	//A button will always be the same sprite sheet
	//Any other UI element will be created differently

	IniParser* pIniParser = Game::GetIniFile();
	std::string spriteFile = pIniParser->GetValueAsString("HUD", "buttonSpriteSheet");

	//Assume a button will always be centered
	m_pButtons.push_back(new Button());
	m_pButtons.back()->CreateButton(
		spriteFile,
		ButtonText,
		x, y,
		CENTER
	);

	//Set buttons call back
	m_pButtons.back()->SetCallback(callBack);
}

void 
GUI::AddHUDElement(std::string HUDText, HudType hudType, int x, int y)
{
	//Assume all hud items are unless defined otherwise
	AddHUDElement(HUDText, hudType, x, y, CENTER);
}

void 
GUI::AddHUDElement(std::string HUDText, HudType hudType, int x, int y, AnchorPoint anchorPoint)
{
	double guiScale = Game::GetIniFile()->GetValueAsDouble("HUD", "guiScale");

	AddHUDElement(HUDText, hudType, x, y, anchorPoint, guiScale);
}

void 
GUI::AddHUDElement(std::string HUDText, HudType hudType, int x, int y, AnchorPoint anchorPoint, double guiScale)
{
	AddHUDElement(HUDText, hudType, x, y, anchorPoint, guiScale, CENTER);
}

void 
GUI::AddHUDElement(std::string HUDText, HudType hudType, int x, int y, AnchorPoint anchorPoint, double guiScale, AnchorPoint textAnchor)
{
	//A HudItem positioning and sprite is defined by the hudType
	m_pHudElements.push_back(new HudItem());
	m_pHudElements.back()->CreateHudItem(
		HUDText,
		x, y,
		hudType,
		anchorPoint,
		guiScale,
		textAnchor
	);
}

void 
GUI::UpdateHud(int index, std::string hudText)
{
	//Set huditems call back
	if (index > static_cast<signed int>(m_pHudElements.size()) - 1 || index < 0) 
	{
		LogManager::Debug("ERROR - Index for Hud element update is out of bounds");
		Game::GetInstance()->Quit();
	}
	else {
		m_pHudElements.at(index)->SetHudString(hudText);
	}
}

void
GUI::Process(double deltaTime)
{
	m_previouslySelected = m_selected;

	//Process buttons
	for (int i = 0; i < static_cast<signed int>(m_pButtons.size()); ++i) {
		if (m_pButtons.at(i)->GetBox()->Contains(&*m_pInput->GetMousePos())) {
			m_selected = i;

			//If clicked
			if (m_pInput->MousePressed(MB1)) {
				Game::GetSoundManager()->PlaySound("reloadNoise", true);

				m_pButtons.at(i)->Activate();
			}
		}

		m_pButtons.at(i)->Process(deltaTime);

		Hover();
	}

	for (std::vector<HudItem*>::iterator it = m_pHudElements.begin(); it != m_pHudElements.end(); ++it) {
		(*it)->Process(deltaTime);
	}
}

void 
GUI::Draw(BackBuffer& backBuffer)
{
	for (std::vector<HudItem*>::iterator it = m_pHudElements.begin(); it != m_pHudElements.end(); ++it) {
		(*it)->DrawStaticUI(backBuffer);
	}

	for (std::vector<Button*>::iterator it = m_pButtons.begin(); it != m_pButtons.end(); ++it) {
		(*it)->DrawButton(backBuffer);
	}
}

std::vector<Button*>*
GUI::GetButtons()
{
	return &m_pButtons;
}

std::vector<HudItem*>*
GUI::GetHudElements()
{
	return &m_pHudElements;
}

void 
GUI::Click()
{
	if (m_selected != -1) {
		Game::GetSoundManager()->PlaySound("reloadNoise", true);
		m_pButtons.at(m_selected)->Activate();
	}
}

void
GUI::DecreaseSelectedElement()
{
	m_selected = (m_selected + 1 >= static_cast<signed int>(m_pButtons.size())) ? 0 : m_selected += 1;
	Hover();
}

void
GUI::IncreaseSelectedElement()
{
	m_selected = (m_selected - 1 < 0) ? static_cast<signed int>(m_pButtons.size()) - 1 : m_selected -= 1;
	Hover();
}

void 
GUI::Hover()
{
	//Change hover state only if you select a new button. 
	//This is to keep the button in a constant hover state for controller support
	if (m_previouslySelected != m_selected) {
		//Play sound
		Game::GetSoundManager()->PlaySound("clickNoise", true);

		//Reset hover states
		for (std::vector<Button*>::iterator it = m_pButtons.begin(); it != m_pButtons.end(); ++it) {
			(*it)->Selected(false);
		}

		m_pButtons[m_selected]->Selected(true);
	}
}
