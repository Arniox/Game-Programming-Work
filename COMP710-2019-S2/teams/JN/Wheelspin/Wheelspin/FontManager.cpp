#include "FontManager.h"

#include "IniParser.h"

FontManager::FontManager()
{
}


FontManager::~FontManager()
{
	std::map<int, TTF_Font*>::iterator iter;
	iter = m_pLoadedFonts.begin();

	m_pLoadedFonts.clear();
}

bool
FontManager::Initialise(IniParser* iniFile)
{
	m_fontPath = iniFile->GetValueAsString("Game", "font");
	return true;
}

TTF_Font*
FontManager::GetFont(const int size)
{
	TTF_Font* pFont = nullptr;

	if (m_pLoadedFonts.find(size) == m_pLoadedFonts.end())
	{
		//Font not loaded, load it
		pFont = TTF_OpenFont(m_fontPath.c_str(), size);
		m_pLoadedFonts[size] = pFont;
	}
	else
	{
		//Font already loaded, retrieve it
		pFont = m_pLoadedFonts[size];
	}

	return pFont;
}
