#include "MenuFrame.h"
#include "SceneManager.h"

MenuFrame::MenuFrame()
: delayCount(0)
, selected(-1)
, previousSelected(-1)
, uiElementArray()
, BUTTON_HOVER_BOOL(false)
{
}
MenuFrame::~MenuFrame()
{
}

void 
MenuFrame::ResetButtonHoverStates(std::vector<UIElement*> uiElementArray)
{
	for (int i = 0; i < static_cast<signed int>(uiElementArray.size()); ++i) {
		uiElementArray.at(i)->GetAnimatedSprite()->SetCurrentPanel(0);
	}
	BUTTON_HOVER_BOOL = false;
}

bool 
MenuFrame::RequestSound()
{
	return BUTTON_HOVER_BOOL;
}
