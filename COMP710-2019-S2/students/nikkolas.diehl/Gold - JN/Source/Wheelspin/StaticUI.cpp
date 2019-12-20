#include "StaticUI.h"

//Locals
#include "sprite.h"
#include "backbuffer.h"
#include "Rectangle.h"
#include "Vector2.h"
#include "game.h"

StaticUI::StaticUI()
{
}
StaticUI::~StaticUI()
{
	delete m_pUIBoundingBox;
	m_pUIBoundingBox = nullptr;

	spriteExists = false;
	delete m_pSprite;
	m_pSprite = nullptr;

	delete m_anchorPoint;
	m_anchorPoint = nullptr;
}

void 
StaticUI::CreateStaticUI(std::string spriteFile, std::string UIText, int x, int y, double scale, AnchorPoint anchorPoint, TTF_Font* font)
{
	//Create empty box
	m_pUIBoundingBox = new Rectangle();
	m_UIText = UIText;

	backBuffer = Game::GetBackBuffer();

	//Set anchor point
	m_anchorPoint = new Vector2(x, y);

	//Initialise the UI element
	if (!spriteFile.empty()) {
		Initialise(backBuffer->CreateSprite(spriteFile.c_str()));
	}

	//Set the size of the UIElement
	SetSize(
		static_cast<int>(m_pSprite->GetWidth() * scale),
		static_cast<int>(m_pSprite->GetHeight() * scale)
	);

	//Set the position based on the anchor point
	double xPos = 0;
	double yPos = 0;

	switch (anchorPoint) {
		case CENTER_TOP:
			xPos = x - ((m_pSprite->GetWidth() * scale) / 2);
			yPos = y;
			break;
		case RIGHT_TOP:
			xPos = x - (m_pSprite->GetWidth() * scale);
			yPos = y;
			break;
		case LEFT_TOP:
			xPos = x;
			yPos = y;
			break;
		case CENTER_BOTTOM:
			xPos = x - ((m_pSprite->GetWidth() * scale) / 2);
			yPos = y - (m_pSprite->GetHeight() * scale);
			break;
		case RIGHT_BOTTOM:
			xPos = x - (m_pSprite->GetWidth() * scale);
			yPos = y - (m_pSprite->GetHeight() * scale);
			break;
		case LEFT_BOTTOM:
			xPos = x;
			yPos = y - (m_pSprite->GetHeight() * scale);
			break;
		case CENTER:
			xPos = x - ((m_pSprite->GetWidth() * scale) / 2);
			yPos = y - ((m_pSprite->GetHeight() * scale) / 2);
			break;
		case CENTER_LEFT:
			xPos = x;
			yPos = y - ((m_pSprite->GetHeight() * scale) / 2);
			break;
		case CENTER_RIGHT:
			xPos = x - (m_pSprite->GetWidth() * scale);
			yPos = y - ((m_pSprite->GetHeight() * scale) / 2);
			break;
		default:
			xPos = 0;
			yPos = 0;
			break;
	}
	SetPos(
		static_cast<int>(xPos),
		static_cast<int>(yPos));

	//Set the text wrap limit in pixels
	m_wrapLimit = static_cast<int>(m_pSprite->GetWidth());

	//Set up sprite data
	m_pSprite->SetSpriteScaler(scale);

	//Set up text font
	m_font = font;
}

void 
StaticUI::DrawStaticUI(BackBuffer& backBuffer)
{
	if (spriteExists) {
		assert(m_pSprite);
		//If the width is custom for UIElement cut off, then set
		//Otherwise just set x and y
		m_pSprite->SetX(static_cast<int>(m_pUIBoundingBox->X()));
		m_pSprite->SetY(static_cast<int>(m_pUIBoundingBox->Y()));

		//Draw with hud true
		m_pSprite->Draw(backBuffer, elementAlpha, true);
	}

	UIElement::DrawUIText(backBuffer, true);
}

bool
StaticUI::Initialise(Sprite* sprite)
{
	assert(sprite);
	m_pSprite = sprite;

	spriteExists = true;

	return (true);
}