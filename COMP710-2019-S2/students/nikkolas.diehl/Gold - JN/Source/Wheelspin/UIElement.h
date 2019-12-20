#ifndef __UIELEMENT_H__
#define __UIELEMENT_H__

class BackBuffer;
class Rectangle;
class Sprite;
class AnimatedSprite;
class Vector2;

#include "SDL_ttf.h"

//Libraries
#include <cassert>
#include <string>
#include <vector>

//Anchor points
enum AnchorPoint {
	CENTER,
	CENTER_TOP,
	RIGHT_TOP,
	LEFT_TOP,
	CENTER_BOTTOM,
	CENTER_LEFT,
	CENTER_RIGHT,
	RIGHT_BOTTOM,
	LEFT_BOTTOM,
};

class UIElement
{
public:
	UIElement();
	~UIElement();

	void SetAlpha(unsigned char a);
	//Set colour with alpha
	void SetTextColour(unsigned char r, unsigned char g, unsigned char b, unsigned char alpha);
	//Set text anchor
	void SetTextAnchor(AnchorPoint textAnchor);

	//Get box
	Rectangle* GetBox();
	//Get sprites
	Sprite* GetStaticSprite();
	AnimatedSprite* GetAnimatedSprite();

	//Update any UI text
	void UpdateElementText(std::string _newText);

	//Get anchor point
	Vector2* GetAnchorPoint();

protected:
	//Creation and drawing
	void DrawUIText(BackBuffer& backBuffer);
	void DrawUIText(BackBuffer& backBuffer, bool HUD);

	//Box creation
	void SetSize(int w, int h);
	void SetPos(int x, int y);

protected:
	std::string m_UIText;

	//Reference copies
	BackBuffer* backBuffer;

	Sprite* m_pSprite;
	AnimatedSprite* m_pAnimatedSprite;
	Rectangle* m_pUIBoundingBox;
	Vector2* m_anchorPoint;

	//Fonts and anchor point for positions
	TTF_Font* m_font;
	AnchorPoint textAnchor;

	//Text colour and text anchor point
	unsigned char color[4];
	unsigned char elementAlpha;

	int m_wrapLimit;
	bool spriteExists;
	bool animatedSpriteExists;


	
};

#endif

