#ifndef __BACKBUFFER_H__
#define __BACKBUFFER_H__

#include "SDL_ttf.h"

// Forward Declarations:
class TextureManager;
class Sprite;
class AnimatedSprite;
class Camera;
struct SDL_Window;
struct SDL_Renderer;

class BackBuffer
{
	//Member Methods:
public:
	BackBuffer();
	~BackBuffer();

	bool Initialise(int width, int height, bool fullScreen, float _M2P, float _P2M);
	void Clear();
	void Present();
	void SetFullscreen(bool fullscreen);
	void SetClearColour(unsigned char r, unsigned char g, unsigned char b);
	void SetDrawColour(unsigned char r, unsigned char g, unsigned char b);
	void SetDrawColour(unsigned char r, unsigned char g, unsigned char b, unsigned char a);
	void DrawSprite(Sprite & sprite, unsigned char alpha, bool HUD);
	void DrawAnimatedSprite(AnimatedSprite& sprite, unsigned char alpha, bool HUD);
	void DrawCircle(int xPos, int yPos, int radius);
	void DrawRectangle(int x1, int y1, int x2, int y2);
	void DrawLine(int x1, int y1, int x2, int y2);
	void DrawPoint(int x, int y);

	void ClearSurfaceCount();
	int GetRenderCount();

	Sprite* CreateSprite(const char* pcFilename);
	Sprite* CopySprite(Sprite* sprite);
	AnimatedSprite* CreateAnimatedSprite(const char* pcFilename, int frameCount, int panelCount, float frameSpeed);

	//Font stuff
	void SetTextColour(unsigned char r, unsigned char g, unsigned char b);
	void SetTextColour(unsigned char r, unsigned char g, unsigned char b, unsigned char a);
	void CreateText(const char* pcText, int WRAP_LIMIT, TTF_Font* font);
	void DrawText(int x, int y, bool HUD);

protected:
	void LogSDLError();

private:
	BackBuffer(const BackBuffer& backBuffer);
	BackBuffer& operator=(const BackBuffer& backBuffer);

	//Member Data:
public:
	unsigned char m_textRed;
	unsigned char m_textGreen;
	unsigned char m_textBlue;
	unsigned char m_textAlpha;
	int m_textWidth;
	int m_textHeight;
protected:
	TextureManager* m_pTextureManager;
	Camera* m_pCam;

	SDL_Window* m_pWindow;
	SDL_Renderer* m_pRenderer;

	int m_width;
	int m_height;

	SDL_Surface* m_textSurface;
	SDL_Texture* m_textTexture;

	unsigned char m_clearRed;
	unsigned char m_clearGreen;
	unsigned char m_clearBlue;

private:
	float M2P;
	float P2M;

	int m_totalDrawnSurfaces;
};

#endif
