#ifndef __UIELEMENT_H__
#define __UIELEMENT_H__

#include "backbuffer.h"
#include "sprite.h"
#include "Rectangle.h"

//Libraries
#include <cassert>
#include <string>

class UIElement
{
public:
	UIElement();
	~UIElement();

	//Creation and drawing
	void CreateAnimatedUIElement(std::string fileName, BackBuffer* backBuffer, std::string UIText, int x, int y, double scale, int frameCount, int panelCount, float frameSpeed, int font);
	void CreateUIElement(std::string fileName, BackBuffer* backBuffer, std::string UIText, int x, int y, double scale, int font);
	void DrawUIElement(BackBuffer& backBuffer, unsigned char alpha, bool Y_TOP);

	//Get box
	Rectangle* GetBox();

	//Get animated sprite
	AnimatedSprite* GetAnimatedSprite();

	//Box creation
	void SetSize(int w, int h);
	void SetPos(int x, int y);

	//Update any UI text
	void UpdateElementText(std::string _newText);

protected:
	bool Initialise_S(Sprite* sprite);
	bool Initialise_An(AnimatedSprite* sprite);

private:
	//UI element text
	std::string mx_s_uiText;
	std::vector<std::string> extraText;
	//UI element position and size
	Rectangle* mx_box;
	//UI element sprite
	Sprite* mx_sprite;
	AnimatedSprite* mx_animatedSprite;
	bool spriteExists;
	bool animatedSpriteExists;
	int wrapLimit;
	int font;

};

#endif //__UIELEMENT_H__

