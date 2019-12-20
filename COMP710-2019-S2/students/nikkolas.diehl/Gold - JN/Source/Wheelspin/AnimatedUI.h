#ifndef __ANIMATEDUI_H__
#define __ANIMATEDUI_H__

//Locals
#include "UIElement.h"
#include "SDL_ttf.h"

//Forward declerations
class BackBuffer;

class AnimatedUI:
	public UIElement
{
public:
	AnimatedUI();
	~AnimatedUI();

	void CreateAnimatedUI(std::string spriteFile, std::string UIText, int x, int y, double scale, AnchorPoint anchorPoint, TTF_Font* font,
		int frameCount, int panelCount, float frameSpeed);
	void DrawAnimatedUI(BackBuffer& backBuffer);

private:
	bool Initialise(AnimatedSprite* sprite);
};

#endif

