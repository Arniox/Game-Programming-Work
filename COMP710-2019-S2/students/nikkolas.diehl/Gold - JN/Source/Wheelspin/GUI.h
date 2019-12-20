#ifndef __GUI_H__
#define __GUI_H__

#include "UIElement.h"
#include "HudItem.h"

//Forward declerations
class StaticUI;
class Button;
class BackBuffer;
class InputManager;

//Includes
#include <string>
#include <vector>
#include <functional>

typedef std::function<void(void)> CallBack;

class GUI
{
public:
	GUI();
	GUI(float _M2P, float _P2M);
	~GUI();

	void AddButton(std::string ButtonText, int x, int y, CallBack callBack);
	void AddHUDElement(std::string HUDText, HudType hudType, int x, int y);
	void AddHUDElement(std::string HUDText, HudType hudType, int x, int y, AnchorPoint anchorPoint);
	void AddHUDElement(std::string HUDText, HudType hudType, int x, int y, AnchorPoint anchorPoint, double guiScale);
	void AddHUDElement(std::string HUDText, HudType hudType, int x, int y, AnchorPoint anchorPoint, double guiScale, AnchorPoint textAnchor);
	void UpdateHud(int index, std::string hudText);
	void Process(double deltaTime);
	void Draw(BackBuffer& backBuffer);

	std::vector<Button*>* GetButtons();
	std::vector<HudItem*>* GetHudElements();

	void Click();
	void IncreaseSelectedElement();
	void DecreaseSelectedElement();

private:
	void Hover();

	std::vector<Button*> m_pButtons;
	std::vector<HudItem*> m_pHudElements;

	InputManager* m_pInput;

	//Selected buttons
	int m_selected;
	int m_previouslySelected;

	float M2P;
	float P2M;
};

#endif
