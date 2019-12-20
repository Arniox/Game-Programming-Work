#ifndef __BITMAPREADER_H__
#define __BITMAPREADER_H__

//Forward Declerations
class Game;
class IniParser;

//Libraries
#include <vector>
#include <string>
#include <SDL.h>
#include <SDL_image.h>

class BitMapReader
{
public:
	BitMapReader();
	~BitMapReader();

	void LoadTexture(std::string pcFileName);

	std::vector<std::vector<int>>* GetPixelArray();

private:
	void ProcessTexture(SDL_Surface* surface);
	
	void Create2DArray();
	void DestroyPixelArray();
	Uint32 GetPixel(SDL_Surface* surface, int x, int y);

private:
	int height = 0;
	int width = 0;
	std::vector<std::vector<int>> m_pPixelArray;
};

#endif

