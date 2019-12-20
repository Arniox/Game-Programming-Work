#ifndef __STATICUI_H__
#define __STATICUI_H__

//Locals
#include "UIElement.h"
#include "SDL_ttf.h"

//Forward Declerations
class BackBuffer;

class StaticUI :
	public UIElement
{
public:
	StaticUI();
	~StaticUI();

	void CreateStaticUI(std::string spriteFile, std::string UIText, int x, int y, double scale, AnchorPoint anchorPoint, TTF_Font* font);
	void DrawStaticUI(BackBuffer& backBuffer);

private:
	bool Initialise(Sprite* sprite);
};

#endif
