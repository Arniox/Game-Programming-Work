#ifndef __HUDITEM_H__
#define __HUDITEM_H__

//Forward declerations
class IniParser;
class FontManager;

//Locals
#include "StaticUI.h"

enum HudType {
	H_STRIP_BLOCK,
	H_SMALL_BACK,
	H_BLOCK,
	H_TRAPESIUM,

	H_SIZE
};

class HudItem:
	public StaticUI
{
public:
	HudItem();
	~HudItem();

	void CreateHudItem(std::string UIText, int x, int y, HudType hudType, AnchorPoint anchorPoint, double guiScale, AnchorPoint textAnchor);
	void DrawHudItem(BackBuffer& backBuffer);
	void Process(double deltaTime);

	void SetHudString(std::string outPut);

private:
	FontManager* m_pFontManager;
	IniParser* m_pIniParser;

	std::string m_pOutPut;
};

#endif

