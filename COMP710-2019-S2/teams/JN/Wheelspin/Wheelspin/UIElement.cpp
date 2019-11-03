#define _CRTDBG_MAP_ALLOC
#include "UIElement.h"

//Locals
#include "backbuffer.h"
#include "Rectangle.h"
#include "sprite.h"
#include "animatedsprite.h"

UIElement::UIElement()
: m_pUIBoundingBox(nullptr)
, m_pSprite(nullptr)
, m_pAnimatedSprite(nullptr)
, m_anchorPoint(nullptr)
, spriteExists(false)
, animatedSpriteExists(false)
, m_wrapLimit(0)
, m_font(nullptr)
, textAnchor(CENTER)
, elementAlpha(255)
{
	m_UIText = "JUNK";
	color[0] = 255;
	color[1] = 255;
	color[2] = 255;
	color[3] = 255;
	textAnchor = CENTER;
}
UIElement::~UIElement()
{	
	if (spriteExists) {
		delete m_pSprite;
	}
	m_pSprite = nullptr;

	if (animatedSpriteExists) {
		delete m_pAnimatedSprite;
	}
	m_pAnimatedSprite = nullptr;
}

void 
UIElement::SetAlpha(unsigned char a)
{
	elementAlpha = a;
}

void
UIElement::DrawUIText(BackBuffer& backBuffer)
{
	//UI text auto drawn as not static
	DrawUIText(backBuffer, false);
}

void 
UIElement::DrawUIText(BackBuffer & backBuffer, bool HUD)
{
	if (!m_UIText.empty()) {
		backBuffer.SetTextColour(color[0], color[1], color[2], color[3]);

		backBuffer.CreateText(m_UIText.c_str(), m_wrapLimit, m_font);

		//Render the text based on the text anchor
		double textBufferX = 0;
		double textBufferY = 0;

		switch (textAnchor) {
		case CENTER_TOP:
			textBufferX = m_pUIBoundingBox->X() + ((m_pUIBoundingBox->W() / 2) - (backBuffer.m_textWidth / 2));
			textBufferY = m_pUIBoundingBox->Y();
			break;
		case RIGHT_TOP:
			textBufferX = m_pUIBoundingBox->X() + (m_pUIBoundingBox->W() - backBuffer.m_textWidth);
			textBufferY = m_pUIBoundingBox->Y();
			break;
		case LEFT_TOP:
			textBufferX = m_pUIBoundingBox->X();
			textBufferY = m_pUIBoundingBox->Y();
			break;
		case CENTER_BOTTOM:
			textBufferX = m_pUIBoundingBox->X() + ((m_pUIBoundingBox->W() / 2) - (backBuffer.m_textWidth / 2));
			textBufferY = m_pUIBoundingBox->Y() + (m_pUIBoundingBox->H() - backBuffer.m_textHeight);
			break;
		case RIGHT_BOTTOM:
			textBufferX = m_pUIBoundingBox->X() + (m_pUIBoundingBox->W() - backBuffer.m_textWidth);
			textBufferY = m_pUIBoundingBox->Y() + (m_pUIBoundingBox->H() - backBuffer.m_textHeight);
			break;
		case LEFT_BOTTOM:
			textBufferX = m_pUIBoundingBox->X();
			textBufferY = m_pUIBoundingBox->Y() + (m_pUIBoundingBox->H() - backBuffer.m_textHeight);
			break;
		case CENTER:
			textBufferX = m_pUIBoundingBox->X() + ((m_pUIBoundingBox->W() / 2) - (backBuffer.m_textWidth / 2));
			textBufferY = m_pUIBoundingBox->Y() + ((m_pUIBoundingBox->H() / 2) - (backBuffer.m_textHeight / 2));
			break;
		case CENTER_LEFT:
			textBufferX = m_pUIBoundingBox->X();
			textBufferY = m_pUIBoundingBox->Y() + ((m_pUIBoundingBox->H() / 2) - (backBuffer.m_textHeight / 2));
			break;
		case CENTER_RIGHT:
			textBufferX = m_pUIBoundingBox->X() + (m_pUIBoundingBox->W() - backBuffer.m_textWidth);
			textBufferY = m_pUIBoundingBox->Y() + ((m_pUIBoundingBox->H() / 2) - (backBuffer.m_textHeight / 2));
			break;
		default:
			textBufferX = 0;
			textBufferY = 0;
			break;
		}

		//Draw text with satic HUD true
		backBuffer.DrawText(
			static_cast<int>(textBufferX),
			static_cast<int>(textBufferY), HUD);
	}
}

void 
UIElement::SetTextColour(unsigned char r, unsigned char g, unsigned char b, unsigned char alpha)
{
	color[0] = r;
	color[1] = g;
	color[2] = b;
	color[3] = alpha;
}

void 
UIElement::SetTextAnchor(AnchorPoint _textAnchor)
{
	textAnchor = _textAnchor;
}

Rectangle* 
UIElement::GetBox()
{
	return m_pUIBoundingBox;
}

Sprite* 
UIElement::GetStaticSprite()
{
	assert(m_pSprite);

	return m_pSprite;
}

AnimatedSprite*
UIElement::GetAnimatedSprite()
{
	assert(m_pAnimatedSprite);

	return m_pAnimatedSprite;
}

void 
UIElement::SetSize(int w, int h)
{
	m_pUIBoundingBox->W() = w;
	m_pUIBoundingBox->H() = h;
}

void 
UIElement::SetPos(int x, int y)
{
	m_pUIBoundingBox->X() = x;
	m_pUIBoundingBox->Y() = y;
}

void 
UIElement::UpdateElementText(std::string _newText)
{
	m_UIText = _newText;
}

Vector2* 
UIElement::GetAnchorPoint()
{
	return m_anchorPoint;
}
