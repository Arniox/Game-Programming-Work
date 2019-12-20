#ifndef __BUTTON_H__
#define __BUTTON_H__

//Forward declerations
class IniParser;
class InputManager;
class FontManager;

//Locals
#include "AnimatedUI.h"
//Libraries
#include <functional>

typedef std::function<void(void)> CallBack;

class Button:
	public AnimatedUI
{
public:
	Button();
	~Button();

	void CreateButton(std::string spriteFile, std::string UIText, int x, int y, AnchorPoint anchor);
	void DrawButton(BackBuffer& backBuffer);
	void Process(double deltaTime);
	void Selected(bool is);
	void SetCallback(CallBack callBack);

	void Activate();

private:
	InputManager* m_pInputManager;
	FontManager* m_pFontManager;
	IniParser* m_pIniParser;

	CallBack m_callBack;

	bool m_selected;
};

#endif