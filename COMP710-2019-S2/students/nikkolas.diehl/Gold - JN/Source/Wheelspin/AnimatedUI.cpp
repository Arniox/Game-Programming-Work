#include "AnimatedUI.h"

//Locals
#include "animatedsprite.h"
#include "backbuffer.h"
#include "Rectangle.h"
#include "Vector2.h"
#include "game.h"

AnimatedUI::AnimatedUI()
{

}
AnimatedUI::~AnimatedUI()
{
	delete m_pUIBoundingBox;
	m_pUIBoundingBox = nullptr;

	animatedSpriteExists = false;
	delete m_pAnimatedSprite;
	m_pAnimatedSprite = nullptr;

	delete m_anchorPoint;
	m_anchorPoint = nullptr;
}

void 
AnimatedUI::CreateAnimatedUI(std::string spriteFile, std::string UIText, int x, int y, double scale, AnchorPoint anchorPoint, TTF_Font* font, int frameCount, int panelCount, float frameSpeed)
{
	//Create empty box
	m_pUIBoundingBox = new Rectangle();
	m_UIText = UIText;

	backBuffer = Game::GetBackBuffer();

	//Set anchor point
	m_anchorPoint = new Vector2(x, y);

	//Initialise the UI element
	if (!spriteFile.empty()) {
		Initialise(backBuffer->CreateAnimatedSprite(spriteFile.c_str(), frameCount, panelCount, frameSpeed));
	}

	//Set the size of the UIElement
	SetSize(
		static_cast<int>(m_pAnimatedSprite->GetFrameWidth() * scale),
		static_cast<int>(m_pAnimatedSprite->GetFrameHeight() * scale)
	);

	//Set the position based on the anchor point
	double xPos = 0;
	double yPos = 0;

	switch (anchorPoint) {
	case CENTER_TOP:
		xPos = x - ((m_pAnimatedSprite->GetFrameWidth() * scale) / 2);
		yPos = y;
		break;
	case RIGHT_TOP:
		xPos = x;
		yPos = y;
		break;
	case LEFT_TOP:
		xPos = x - (m_pAnimatedSprite->GetFrameWidth() * scale);
		yPos = y;
		break;
	case CENTER_BOTTOM:
		xPos = x - ((m_pAnimatedSprite->GetFrameWidth() * scale) / 2);
		yPos = y - (m_pAnimatedSprite->GetFrameHeight() * scale);
		break;
	case RIGHT_BOTTOM:
		xPos = x;
		yPos = y - (m_pAnimatedSprite->GetFrameHeight() * scale);
		break;
	case LEFT_BOTTOM:
		xPos = x - (m_pAnimatedSprite->GetFrameWidth() * scale);
		yPos = y - (m_pAnimatedSprite->GetFrameHeight() * scale);
		break;
	case CENTER:
		xPos = x - ((m_pAnimatedSprite->GetFrameWidth() * scale) / 2);
		yPos = y - ((m_pAnimatedSprite->GetFrameHeight() * scale) / 2);
		break;
	case CENTER_LEFT:
		xPos = x - (m_pAnimatedSprite->GetFrameWidth() * scale);
		yPos = y - ((m_pAnimatedSprite->GetFrameHeight() * scale) / 2);
		break;
	case CENTER_RIGHT:
		xPos = x;
		yPos = y - ((m_pAnimatedSprite->GetFrameHeight() * scale) / 2);
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
	m_wrapLimit = static_cast<int>(m_pAnimatedSprite->GetFrameWidth());

	//Set up sprite data
	m_pAnimatedSprite->SetSpriteScaler(scale);
	m_pAnimatedSprite->SetCurrentPanel(0);

	//Set up text font
	m_font = font;
}

void 
AnimatedUI::DrawAnimatedUI(BackBuffer& backBuffer)
{
	if (animatedSpriteExists) {
		assert(m_pAnimatedSprite);
		//If the width is custom for UIElement cut off, then set
		//Otherwise just set x and y
		m_pAnimatedSprite->SetX(static_cast<int>(m_pUIBoundingBox->X()));
		m_pAnimatedSprite->SetY(static_cast<int>(m_pUIBoundingBox->Y()));

		//Draw animated UI with HUD true
		m_pAnimatedSprite->Draw(backBuffer, elementAlpha, true);
	}

	UIElement::DrawUIText(backBuffer, true);
}

bool
AnimatedUI::Initialise(AnimatedSprite* sprite)
{
	assert(sprite);
	m_pAnimatedSprite = sprite;

	animatedSpriteExists = true;

	return (true);
}
