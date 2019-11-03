
//Locals
#include "BitMapReader.h"
#include "game.h"
#include "IniParser.h"
#include "logmanager.h"

BitMapReader::BitMapReader()
{
}

BitMapReader::~BitMapReader()
{
	DestroyPixelArray();
}

void
BitMapReader::LoadTexture(std::string pcFileName)
{
	SDL_Surface* m_pImageSurface = IMG_Load(pcFileName.c_str());

	height = m_pImageSurface->h;
	width = m_pImageSurface->w;

	Create2DArray();
	ProcessTexture(m_pImageSurface);
}

std::vector<std::vector<int>>*
BitMapReader::GetPixelArray()
{
	return &m_pPixelArray;
}

void 
BitMapReader::ProcessTexture(SDL_Surface* surface)
{
	for (int y = 0; y < height; ++y)
	{
		for (int x = 0; x < width; ++x)
		{
			SDL_Color colorAt;
			Uint32 data = GetPixel(surface, x, y);
			SDL_GetRGB(data, surface->format, &colorAt.r, &colorAt.g, &colorAt.b);
			
			int pixel = std::stoi(std::to_string(colorAt.r) + std::to_string(colorAt.g) + std::to_string(colorAt.b));

			m_pPixelArray.at(y).at(x) = pixel;
		}
	}
}

Uint32 
BitMapReader::GetPixel(SDL_Surface* surface, int x, int y)
{
	int bpp = surface->format->BytesPerPixel;
	//Address p is the pixel we want
	Uint8 *p = static_cast<Uint8*>(surface->pixels) + y * surface->pitch + x * bpp;

	switch (bpp)
	{
	case 1:
		return *p;
		break;
	case 2:
		return *(Uint16 *)p;
		break;
	case 3:
		if (SDL_BYTEORDER == SDL_BIG_ENDIAN) {
			return p[0] << 16 | p[1] << 8 | p[2];
		}
		else {
			return p[0] | p[1] << 8 | p[2] << 16;
		}
		break;
	case 4:
		return *(Uint32 *)p;
		break;
	default:
		return 0; //Avoids warnings
	}
}

void
BitMapReader::Create2DArray()
{
	for (int y = 0; y < height; ++y)
	{
		std::vector<int> nu;

		for (int x = 0; x < width; ++x)
		{
			nu.push_back(0);
		}

		m_pPixelArray.push_back(nu);
	}
}

void 
BitMapReader::DestroyPixelArray()
{
	for (std::vector<std::vector<int>>::iterator it = m_pPixelArray.begin(); it != m_pPixelArray.end(); ++it)
	{
		(*it).clear();
		(*it).shrink_to_fit();
	}

	m_pPixelArray.clear();
	m_pPixelArray.shrink_to_fit();
}
