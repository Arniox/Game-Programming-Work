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
#include "game.h"
#include "IniParser.h"
#include "Camera.h"

// Library includes:
#include <SDL.h>
#include <SDL_image.h>
#include <cassert>
#include <string>

BackBuffer::BackBuffer()
: m_pTextureManager(0)
, m_pCam(nullptr)
, m_pWindow(0)
, m_pRenderer(0)
, m_width(0)
, m_height(0)
, m_clearRed(0)
, m_clearGreen(0)
, m_clearBlue(0)
, m_textRed(0)
, m_textGreen(0)
, m_textBlue(0)
, m_textAlpha(0)
, m_totalDrawnSurfaces(0)
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
BackBuffer::Initialise(int width, int height, bool fullScreen, float _M2P, float _P2M)
{
	m_width = width;
	m_height = height;

	M2P = _M2P;
	P2M = _P2M;

	m_pCam = Game::GetCamera();

	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK) < 0)
	{
		LogSDLError();
		return (false);
	}
	else
	{
		std::string title = Game::GetIniFile()->GetValueAsString("Game", "title");
		m_pWindow = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN);

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
	ClearSurfaceCount();
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
BackBuffer::DrawSprite(Sprite& sprite, unsigned char alpha, bool HUD)
{
	bool draw = true;
	//Create destination point
	SDL_Rect dest;

	if (!HUD)
	{
		// Check if the sprite is in bounds of the camera
		int xPos = sprite.GetX();
		int yPos = sprite.GetY();

		// Check if the sprite is currently in the window frame
		if (m_pCam->InBounds(xPos, yPos))
		{
			dest.x = xPos + m_pCam->x;
			dest.y = yPos + m_pCam->y;
		}
		else
			draw = false;
	}
	else
	{
		dest.x = sprite.GetX();
		dest.y = sprite.GetY();
	}

	if (draw)
	{
		SDL_Rect src;
		//Rotate around 0, 0 for walls. Everything is centrally rotated.
		SDL_Point rotPoint = { 0,0 };

		src.x = sprite.GetSheetX();
		src.y = sprite.GetSheetY();
		src.w = sprite.GetWidth();
		src.h = sprite.GetHeight();

		dest.w = static_cast<int>(sprite.GetWidth() * sprite.GetSpriteScaler());
		dest.h = static_cast<int>(sprite.GetHeight() * sprite.GetSpriteScaler());

		//Add alpha before drawing
		SDL_SetTextureAlphaMod(sprite.GetTexture()->GetTexture(), alpha);

		SDL_RenderCopyEx(m_pRenderer, sprite.GetTexture()->GetTexture(),
			&src, &dest, sprite.GetAngle(),
			(sprite.GetRotate() ? NULL : &rotPoint), sprite.flipSprite);

		m_totalDrawnSurfaces++;
	}
}

void
BackBuffer::DrawAnimatedSprite(AnimatedSprite& animatedSprite, unsigned char alpha, bool HUD)
{
	bool draw = true;
	// Draw the animated sprite
	SDL_Rect dest;

	if (!HUD)
	{
		int xPos = animatedSprite.GetX();
		int yPos = animatedSprite.GetY();

		// Check if the animated sprite is currently in the window frame
		if (m_pCam->InBounds(xPos, yPos))
		{
			dest.x = xPos + m_pCam->x;
			dest.y = yPos + m_pCam->y;
		}
		else
			draw = false;
	}
	else
	{
		dest.x = animatedSprite.GetX();
		dest.y = animatedSprite.GetY();
	}

	if (draw)
	{

		SDL_Rect src;
		//Rotate around 0, 0 for walls. Everything is centrally rotated.
		SDL_Point rotPoint = { 0,0 };

		src.x = animatedSprite.GetFrameCoords();
		src.y = animatedSprite.GetCurrentPanel();
		src.w = animatedSprite.GetFrameWidth();
		src.h = animatedSprite.GetFrameHeight();

		dest.w = static_cast<int>(animatedSprite.GetFrameWidth() * animatedSprite.GetSpriteScaler());
		dest.h = static_cast<int>(animatedSprite.GetFrameHeight() * animatedSprite.GetSpriteScaler());

		//Add alpha before drawing
		SDL_SetTextureAlphaMod(animatedSprite.GetTexture()->GetTexture(), alpha);

		SDL_RenderCopyEx(m_pRenderer, animatedSprite.GetTexture()->GetTexture(),
			&src, &dest, animatedSprite.GetAngle(),
			(animatedSprite.GetRotate() ? NULL : &rotPoint), animatedSprite.flipSprite);

		m_totalDrawnSurfaces++;
	}
}

// Uses https://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm
void
BackBuffer::DrawCircle(int xPos, int yPos, int radius)
{
	bool draw = true;

	if (!m_pCam->InBounds(xPos, yPos)) {
		draw = false;
	}

	if (draw) 
	{
		int cX = xPos + m_pCam->x;
		int cY = yPos + m_pCam->y;
		int diameter = radius * 2;
		int x = radius - 1;
		int y = 0;
		int dx = 1;
		int dy = 1;
		int err = dx - diameter;

		while (x >= y)
		{
			SDL_RenderDrawPoint(m_pRenderer, cX + x, cY - y);
			SDL_RenderDrawPoint(m_pRenderer, cX + x, cY + y);
			SDL_RenderDrawPoint(m_pRenderer, cX - x, cY - y);
			SDL_RenderDrawPoint(m_pRenderer, cX - x, cY + y);
			SDL_RenderDrawPoint(m_pRenderer, cX + y, cY + x);
			SDL_RenderDrawPoint(m_pRenderer, cX - y, cY - x);
			SDL_RenderDrawPoint(m_pRenderer, cX - y, cY + x);
			SDL_RenderDrawPoint(m_pRenderer, cX + y, cY - x);

			if (err <= 0)
			{
				++y;
				err += dy;
				dy += 2;
			}

			if (err > 0)
			{
				--x;
				dx += 2;
				err += dx - diameter;
			}
		}
	}
}

void
BackBuffer::DrawRectangle(int x1, int y1, int x2, int y2)
{
	bool draw = true;

	if (!(m_pCam->InBounds(x1, y1)) && !(m_pCam->InBounds(x2, y2))) {
		draw = false;
	}

	if (draw) 
	{
		SDL_Rect fillRect;

		fillRect.x = x1 + m_pCam->x;
		fillRect.y = y1 + m_pCam->y;
		fillRect.w = (x2 - x1);
		fillRect.h = (y2 - y1);

		SDL_RenderFillRect(m_pRenderer, &fillRect);
	}
}

void
BackBuffer::DrawLine(int x1, int y1, int x2, int y2)
{
	bool draw = true;

	if (!(m_pCam->InBounds(x1, y1)) && !(m_pCam->InBounds(x2, y2))) {
		draw = false;
	}

	if (draw) 
	{
		SDL_RenderDrawLine(m_pRenderer, x1 + m_pCam->x, y1 + m_pCam->y, x2 + m_pCam->x, y2 + m_pCam->y);
	}
}

void 
BackBuffer::DrawPoint(int x, int y)
{
	bool draw = true;

	if (!(m_pCam->InBounds(x, y))) {
		draw = false;
	}

	if (draw) 
	{
		SDL_RenderDrawPoint(m_pRenderer, x + m_pCam->x, y + m_pCam->y);
	}
}

void 
BackBuffer::ClearSurfaceCount()
{
	m_totalDrawnSurfaces = 0;
}

int BackBuffer::GetRenderCount()
{
	return m_totalDrawnSurfaces;
}

void
BackBuffer::LogSDLError()
{
	LogManager::Debug(SDL_GetError());
	LogManager::GetInstance().DestroyInstance();
}

void BackBuffer::SetTextColour(unsigned char r, unsigned char g, unsigned char b)
{
	m_textRed = r;
	m_textGreen = g;
	m_textBlue = b;
}

void 
BackBuffer::SetTextColour(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
	m_textRed = r;
	m_textGreen = g;
	m_textBlue = b;
	m_textAlpha = a;
}

void 
BackBuffer::CreateText(const char* pcText, int WRAP_LIMIT, TTF_Font* font)
{
	//Declare
	SDL_Color color;
	color.r = m_textRed;
	color.g = m_textGreen;
	color.b = m_textBlue;
	color.a = m_textAlpha;

	//Pass objects to create the text
	m_textSurface = TTF_RenderText_Blended_Wrapped(font, pcText, color, WRAP_LIMIT);
	m_textWidth = m_textSurface->w;
	m_textHeight = m_textSurface->h;

	m_textTexture = SDL_CreateTextureFromSurface(m_pRenderer, m_textSurface);
}

void BackBuffer::DrawText(int x, int y, bool HUD)
{
	bool draw = true;

	if (!HUD) 
	{
		if (!(m_pCam->InBounds(x, y))) {
			draw = false;
		}
	}
	
	if (draw) 
	{
		int w, h;
		SDL_QueryTexture(m_textTexture, 0, 0, &w, &h);

		//Destination
		SDL_Rect destinationRectangle;
		if (!HUD)
		{
			//need to add achoring
			destinationRectangle.x = x + m_pCam->x;
			destinationRectangle.y = y + m_pCam->y;
		}
		else
		{
			//need to add achoring
			destinationRectangle.x = x;
			destinationRectangle.y = y;
		}

		destinationRectangle.w = w;
		destinationRectangle.h = h;

		//Render out the text
		SDL_RenderCopy(m_pRenderer, m_textTexture, 0, &destinationRectangle);

		//Destroy texture to clean memory and free the surface
		SDL_DestroyTexture(m_textTexture);
		SDL_FreeSurface(m_textSurface);

		m_totalDrawnSurfaces++;
	}
	
}

Sprite*
BackBuffer::CreateSprite(const char* pcFilename)
{
	assert(m_pTextureManager);

	Texture* pTexture = m_pTextureManager->GetTexture(pcFilename);

	Sprite* pSprite = new Sprite();
	if (!pSprite->Initialise(*pTexture))
	{
		LogManager::Debug("Sprite Failed to Create!");
		LogManager::GetInstance().DestroyInstance();

	}

	return (pSprite);
}

Sprite* 
BackBuffer::CopySprite(Sprite* sprite)
{
	assert(sprite);

	if (sprite) {
		Sprite* pSprite = new Sprite();
		if (!pSprite->Initialise(*sprite->GetTexture())) {
			LogManager::Debug("ERROR - Sprite Failed to create!");
		}

		return (pSprite);
	}
	else {
		LogManager::Debug("ERROR - Sprite does not exist!");
		return NULL;
	}
}

AnimatedSprite*
BackBuffer::CreateAnimatedSprite(const char* pcFilename, int frameCount, int panelCount, float frameSpeed)
{
	assert(m_pTextureManager);
	Texture* pTexture = m_pTextureManager->GetTexture(pcFilename);

	AnimatedSprite* pAnimatedSprite = new AnimatedSprite();

	if (!pAnimatedSprite->Initialise(*pTexture))
	{
		LogManager::Debug("Sprite Failed to Create!");
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
