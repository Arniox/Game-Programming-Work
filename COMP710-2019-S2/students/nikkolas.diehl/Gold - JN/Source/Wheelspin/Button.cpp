#include "Button.h"

//Locals
#include "backbuffer.h"
#include "IniParser.h"
#include "InputManager.h"
#include "FontManager.h"
#include "animatedsprite.h"
#include "game.h"
#include "Rectangle.h"
#include "Vector2.h"

//Libraries
#include <memory>

Button::Button()
: m_selected(false)
{
}
Button::~Button()
{
	m_pFontManager = nullptr;
	m_pIniParser = nullptr;
	m_callBack = nullptr;
}

void 
Button::CreateButton(std::string spriteFile, std::string UIText, int x, int y, AnchorPoint anchor)
{
	//Copy references
	m_pFontManager = Game::GetFontManager();
	m_pIniParser = Game::GetIniFile();

	double guiScale = m_pIniParser->GetValueAsDouble("HUD", "guiScale");
	TTF_Font* font = m_pFontManager->GetFont(static_cast<const int>(m_pIniParser->GetValueAsInt("HUD", "fontSize") * guiScale));

	int frameCount = m_pIniParser->GetValueAsInt("HUD", "buttonFrameCount");
	int panelCount = m_pIniParser->GetValueAsInt("HUD", "buttonPanelCount");
	float buttonFrameSpeed = 0.0f;

	//Create the animatedUI element
	AnimatedUI::CreateAnimatedUI(spriteFile, UIText, x, y, guiScale, anchor, font, frameCount, panelCount, buttonFrameSpeed);

	//Set font data
	SetTextAnchor(CENTER);
	SetTextColour(
		m_pIniParser->GetValueAsChar("HUD", "textColorR"),
		m_pIniParser->GetValueAsChar("HUD", "textColorG"),
		m_pIniParser->GetValueAsChar("HUD", "textColorB"),
		m_pIniParser->GetValueAsChar("HUD", "textColorA")
	);
}

void 
Button::DrawButton(BackBuffer& backBuffer)
{
	AnimatedUI::DrawAnimatedUI(backBuffer);
}

void 
Button::Process(double deltaTime)
{
	//Switch panels if hovered
	if (m_selected) {
		m_pAnimatedSprite->SetCurrentPanel(1);
	}
	else {
		m_pAnimatedSprite->SetCurrentPanel(0);
	}
}

void 
Button::Selected(bool is)
{
	m_selected = is;
}

void
Button::SetCallback(CallBack callBack)
{
	m_callBack = callBack;
}

void
Button::Activate()
{
	if (m_callBack) {
		m_callBack();
	}
}
