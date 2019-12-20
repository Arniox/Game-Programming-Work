#include "HudItem.h"

//Locals
#include "backbuffer.h"
#include "IniParser.h"
#include "FontManager.h"
#include "sprite.h"
#include "game.h"
#include "Rectangle.h"
#include "Vector2.h"
#include "logmanager.h"

//Libraries
#include <memory>

HudItem::HudItem()
:m_pOutPut(" ")
{
}
HudItem::~HudItem()
{
	m_pFontManager = nullptr;
	m_pIniParser = nullptr;
}

void 
HudItem::CreateHudItem(std::string UIText, int x, int y, HudType hudType, AnchorPoint anchorPoint, double guiScale, AnchorPoint textAnchor)
{
	//Copy references
	m_pFontManager = Game::GetFontManager();
	m_pIniParser = Game::GetIniFile();

	TTF_Font* font = m_pFontManager->GetFont(static_cast<const int>(m_pIniParser->GetValueAsInt("HUD", "fontSize") * guiScale));

	std::string spriteFile = "";

	//Create static UI element
	//Assume centered UI Elements
	//Set font data
	switch (hudType) {
	case H_STRIP_BLOCK:
		spriteFile = m_pIniParser->GetValueAsString("HUD", "hudStripBlock");
		StaticUI::CreateStaticUI(spriteFile, UIText, x, y, 1.0, anchorPoint, font);
		SetTextAnchor(CENTER_TOP);
		SetAlpha(127);
		break;
	case H_SMALL_BACK:
		spriteFile = m_pIniParser->GetValueAsString("HUD", "hudSmallBlock");
		StaticUI::CreateStaticUI(spriteFile, UIText, x, y, guiScale, anchorPoint, font);
		SetTextAnchor(textAnchor);
		break;
	case H_BLOCK:
		spriteFile = m_pIniParser->GetValueAsString("HUD", "hudBlock");
		StaticUI::CreateStaticUI(spriteFile, UIText, x, y, guiScale, anchorPoint, font);
		SetTextAnchor(textAnchor);
		break;
	case H_TRAPESIUM:
		spriteFile = m_pIniParser->GetValueAsString("HUD", "hudTrapesium");
		StaticUI::CreateStaticUI(spriteFile, UIText, x, y, guiScale, anchorPoint, font);
		SetTextAnchor(textAnchor);
		break;
	default:
		LogManager::Debug("ERROR - hudType does not exist. Could not create new hud item");
		Game::GetInstance()->Quit();
	}
	SetTextColour(
		m_pIniParser->GetValueAsChar("HUD", "textColorR"),
		m_pIniParser->GetValueAsChar("HUD", "textColorG"),
		m_pIniParser->GetValueAsChar("HUD", "textColorB"),
		m_pIniParser->GetValueAsChar("HUD", "textColorA")
	);
}

void 
HudItem::DrawHudItem(BackBuffer & backBuffer)
{
	StaticUI::DrawStaticUI(backBuffer);
}

void 
HudItem::Process(double deltaTime)
{
	//Call back activate. Probably change text
	if (m_pOutPut != "") {
		//Assume that call back should return text to update the hud item text
		UpdateElementText(m_pOutPut);
	}
}

void 
HudItem::SetHudString(std::string outPut)
{
	m_pOutPut = outPut;
}
