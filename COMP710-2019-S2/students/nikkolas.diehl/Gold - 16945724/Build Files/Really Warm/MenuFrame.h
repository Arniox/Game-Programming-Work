#ifndef __MENUFRAME_H__
#define __MENUFRAME_H__

//Locals
#include "backbuffer.h"
#include "iniParser.h"
#include "fmod.h"
#include "inputhandler.h"
#include "UIElement.h"

class MenuFrame
{
public:
	MenuFrame();
	~MenuFrame();

	int selected = 0;
	int previousSelected = 0;
	bool RequestSound();

protected:
	void ResetButtonHoverStates(std::vector<UIElement*> uiElementArray);

protected:
	const double BUTTON_SCALE = 1.0;
	const int FRAME_COUNT = 1;
	const float FRAME_SPEED = 0;
	const int PANEL_COUNT = 2;
	const int BTN_FONT = 2;

	const int MAX_DELAY = 7;
	int delayCount = 0;

	enum ButtonPanels {
		UNCLICKED = 0,
		CLICKED = 1
	};
	std::vector<UIElement*> uiElementArray;

	//Sound bool
	bool BUTTON_HOVER_BOOL = false;
};

#endif
