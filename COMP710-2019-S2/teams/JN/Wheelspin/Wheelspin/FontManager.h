#ifndef __FONT_MANAGER_H__
#define __FONT_MANAGER_H__

#include "SDL_ttf.h"
#include <string>
#include <map>

class IniParser;

class FontManager
{
public:
	FontManager();
	~FontManager();

	bool Initialise(IniParser* iniFile);
	TTF_Font* GetFont(const int size);

private:
	FontManager(const FontManager& fontManager);
	FontManager& operator=(const FontManager& fontManager);

protected:
	std::string m_fontPath;
	std::map<int, TTF_Font*> m_pLoadedFonts;
};

#endif // !__FONT_MANAGER_H__