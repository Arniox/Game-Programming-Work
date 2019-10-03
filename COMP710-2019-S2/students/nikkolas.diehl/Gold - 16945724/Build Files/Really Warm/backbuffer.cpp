#define _CRTDBG_MAP_ALLOC
// COMP710 GP 2D Framework 2019

// This include:
#include "backbuffer.h"

// Local includes:
#include "logmanager.h"
#include "texturemanager.h"
#include "sprite.h"
#include "animatedsprite.h"
#include "texture.h"

// Library includes:
#include <SDL.h>
#include <SDL_image.h>
#include <cassert>

BackBuffer::BackBuffer()
: m_pTextureManager(0)
, m_pWindow(0)
, m_pRenderer(0)
, m_width(0)
, m_height(0)
, m_clearRed(0)
, m_clearGreen(0)
, m_clearBlue(0)
, fonts()
, m_textRed(0)
, m_textGreen(0)
, m_textBlue(0)
{
}

BackBuffer::~BackBuffer()
{
	SDL_DestroyRenderer(m_pRenderer);
	m_pRenderer = nullptr;

	SDL_DestroyWindow(m_pWindow);
	m_pWindow = nullptr;

	//Delete Texture manager
	delete m_pTextureManager;
	m_pTextureManager = nullptr;

	//Clear sprites

	IMG_Quit();
	SDL_Quit();
}

bool 
BackBuffer::Initialise(int width, int height, bool fullScreen)
{
	m_width = width;
	m_height = height;

	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK) < 0)
	{
		LogSDLError();
		return (false);
	}
	else
	{
		m_pWindow = SDL_CreateWindow("COMP710 2D Game Framework", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN);
	
		if (m_pWindow == 0)
		{
			LogSDLError();
			return (false);
		}
		else
		{
			m_pRenderer = SDL_CreateRenderer(m_pWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC); 

			if (m_pRenderer == 0)
			{
				LogSDLError();
				return (false);
			}
			else
			{
				int imageResult = IMG_Init(IMG_INIT_PNG);

				if (imageResult == 0)
				{
					LogSDLError();
					return (false);
				}
			}
		}
	}

	SetFullscreen(fullScreen);
	SDL_Delay(1000);

	TTF_Init();
	fonts[0] = TTF_OpenFont("assets/space Xrebron.ttf", 40); //Title Font
	fonts[1] = TTF_OpenFont("assets/space Xrebron.ttf", 20); //FPS font
	fonts[2] = TTF_OpenFont("assets/nasalization-rg.ttf", 20); //Spacy font

	m_pTextureManager = new TextureManager();
	assert(m_pTextureManager);
	m_pTextureManager->Initialise(m_pRenderer);

	return (true);
}

void 
BackBuffer::SetFullscreen(bool fullscreen)
{
	if (fullscreen)
	{
//		SDL_SetWindowFullscreen(m_pWindow, SDL_WINDOW_FULLSCREEN | SDL_WINDOW_ALWAYS_ON_TOP);
		SDL_SetWindowFullscreen(m_pWindow, SDL_WINDOW_FULLSCREEN_DESKTOP | SDL_WINDOW_ALWAYS_ON_TOP);
		SDL_SetHint(SDL_HINT_VIDEO_MINIMIZE_ON_FOCUS_LOSS, "0");
		SDL_SetWindowSize(m_pWindow, m_width, m_height);
	}
	else
	{
		SDL_SetWindowFullscreen(m_pWindow, 0);
	}
}

void 
BackBuffer::Clear()
{
	SDL_SetRenderDrawColor(m_pRenderer, m_clearRed, m_clearGreen, m_clearBlue, 0xff);
	SDL_RenderClear(m_pRenderer);
}

void
BackBuffer::Present()
{
	SDL_RenderPresent(m_pRenderer);
}

void
BackBuffer::SetDrawColour(unsigned char r, unsigned char g, unsigned char b)
{
	SetDrawColour(r, g, b, 0xFF);
}

void
BackBuffer::SetDrawColour(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
	SDL_SetRenderDrawColor(m_pRenderer, r, g, b, a);
}

void 
BackBuffer::DrawSprite(Sprite& sprite, unsigned char alpha)
{
	//Create destination point
	SDL_Rect dest;
	SDL_Rect src;
	//Rotate around 0, 0 for walls. Everything is centrally rotated.
	SDL_Point rotPoint = { 0,0 };

	src.x = sprite.GetSheetX();
	src.y = sprite.GetSheetY();
	src.w = sprite.GetWidth();
	src.h = sprite.GetHeight();

	dest.x = sprite.GetX();
	dest.y = sprite.GetY();
	dest.w = static_cast<int>(sprite.GetWidth() * sprite.GetSpriteScaler());
	dest.h = static_cast<int>(sprite.GetHeight() * sprite.GetSpriteScaler());

	//Add alpha before drawing
	SDL_SetTextureAlphaMod(sprite.GetTexture()->GetTexture(), alpha);

	SDL_RenderCopyEx(m_pRenderer, sprite.GetTexture()->GetTexture(), 
					 &src, &dest, sprite.GetAngle(), 
					 (sprite.GetRotate() ? NULL : &rotPoint), sprite.flipSprite);
}

void BackBuffer::DrawAnimatedSprite(AnimatedSprite & animatedSprite, unsigned char alpha)
{
	SDL_Rect dest;
	SDL_Rect src;
	//Rotate around 0, 0 for walls. Everything is centrally rotated.
	SDL_Point rotPoint = { 0,0 };

	src.x = animatedSprite.GetFrameCoords();
	src.y = animatedSprite.GetCurrentPanel();
	src.w = animatedSprite.GetFrameWidth();
	src.h = animatedSprite.GetFrameHeight();

	dest.x = animatedSprite.GetX();
	dest.y = animatedSprite.GetY();
	dest.w = static_cast<int>(animatedSprite.GetFrameWidth() * animatedSprite.GetSpriteScaler());
	dest.h = static_cast<int>(animatedSprite.GetFrameHeight() * animatedSprite.GetSpriteScaler());

	//Create flip
	SDL_RendererFlip flip = SDL_FLIP_NONE;

	//Add alpha before drawing
	SDL_SetTextureAlphaMod(animatedSprite.GetTexture()->GetTexture(), alpha);

	SDL_RenderCopy(m_pRenderer, animatedSprite.GetTexture()->GetTexture(), &src, &dest);
	SDL_RenderCopyEx(m_pRenderer, animatedSprite.GetTexture()->GetTexture(),
					 &src, &dest, animatedSprite.GetAngle(),
					 (animatedSprite.GetRotate() ? NULL : &rotPoint), flip);
	
}

void
BackBuffer::DrawRectangle(int x1, int y1, int x2, int y2)
{
	SDL_Rect fillRect;
	
	fillRect.x = x1;
	fillRect.y = y1;
	fillRect.w = x2 - x1;
	fillRect.h = y2 - y1;

	SDL_RenderFillRect(m_pRenderer, &fillRect);
}

void
BackBuffer::DrawLine(int x1, int y1, int x2, int y2)
{
	SDL_RenderDrawLine(m_pRenderer, x1, y1, x2, y2);
}

void 
BackBuffer::LogSDLError()
{
	LogManager::GetInstance().Log(SDL_GetError());
	LogManager::GetInstance().DestroyInstance();
}

void BackBuffer::SetTextColour(unsigned char r, unsigned char g, unsigned char b)
{
	m_textRed = r;
	m_textGreen = g;
	m_textBlue = b;
}

void BackBuffer::DrawText(int x, int y, const char * pcText, int font, bool CENTER, int WRAP_LIMIT)
{
	//Declare
	SDL_Color color;
	color.r = m_textRed;
	color.g = m_textGreen;
	color.b = m_textBlue;
	color.a = 255;

	//Pass objects to create the text
	SDL_Surface* surface = TTF_RenderText_Blended_Wrapped(fonts[font], pcText, color, WRAP_LIMIT);
	int lengthOfCharacter = surface->w / sizeof(pcText);

	SDL_Texture* texture = SDL_CreateTextureFromSurface(m_pRenderer, surface);
	int w, h;
	SDL_QueryTexture(texture, 0, 0, &w, &h);

	//Destination
	SDL_Rect destinationRectangle;
	(!CENTER ? destinationRectangle.x = x : destinationRectangle.x = x - ((sizeof(pcText) / 2) * lengthOfCharacter));
	destinationRectangle.y = y;
	destinationRectangle.w = w;
	destinationRectangle.h = h;

	//Render out the text
	SDL_RenderCopy(m_pRenderer, texture, 0, &destinationRectangle);

	//Destroy texture to clean memory and free the surface
	SDL_DestroyTexture(texture);
	SDL_FreeSurface(surface);
}

Sprite* 
BackBuffer::CreateSprite(const char* pcFilename)
{
	assert(m_pTextureManager);

	Texture* pTexture = m_pTextureManager->GetTexture(pcFilename);

	Sprite* pSprite = new Sprite();
	if (!pSprite->Initialise(*pTexture))
	{
		LogManager::GetInstance().Log("Sprite Failed to Create!");
		LogManager::GetInstance().DestroyInstance();

	}

	return (pSprite);
}

AnimatedSprite*
BackBuffer::CreateAnimatedSprite(const char* pcFilename, int frameCount, int panelCount, float frameSpeed)
{
	assert(m_pTextureManager);
	Texture* pTexture = m_pTextureManager->GetTexture(pcFilename);

	AnimatedSprite* pAnimatedSprite = new AnimatedSprite();

	if (!pAnimatedSprite->Initialise(*pTexture))
	{
		LogManager::GetInstance().Log("Sprite Failed to Create!");
		LogManager::GetInstance().DestroyInstance();
	}
	else 
	{
		pAnimatedSprite->SetFrameWidth(pTexture->GetWidth() / frameCount);
		pAnimatedSprite->SetFrameHeight(pTexture->GetHeight() / panelCount);

		pAnimatedSprite->SetFrameSpeed(frameSpeed);
		for (int i = 0; i < frameCount; ++i) {
			pAnimatedSprite->AddFrame(i * (pTexture->GetWidth() / frameCount));
		}
	}

	return (pAnimatedSprite);
}

void 
BackBuffer::SetClearColour(unsigned char r, unsigned char g, unsigned char b)
{
	m_clearRed = r;
	m_clearGreen = g;
	m_clearBlue = b;
}
