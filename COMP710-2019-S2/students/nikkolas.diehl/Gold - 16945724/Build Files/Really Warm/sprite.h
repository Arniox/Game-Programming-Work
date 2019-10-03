#ifndef __SPRITE_H__
#define __SPRITE_H__

#include <SDL.h>
#include <SDL_image.h>

// Forward Declarations:
class BackBuffer;
class Texture;

class Sprite
{
	//Member Methods:
public:
	Sprite();
	~Sprite();

	bool Initialise(Texture& texture);

	void Process(float deltaTime);
	void Draw(BackBuffer& backbuffer, unsigned char alpha);

	void SetX(int x);
	void SetY(int y);
	void RotateNull(bool rotate);
	bool GetRotate();

	int GetX() const;
	int GetY() const;

	void SetAngle(double angle);
	float GetAngle() const;

	void SetLength(int legnth);
	void SetHeight(int height);

	int GetWidth() const;
	int GetHeight() const;

	//Dynamic sprite stuff
	//Set
	void SetSheetX(int _x);
	void SetSheetY(int _y);
	void SetSpriteScaler(double _scaler);
	//Get
	int GetSheetX() const;
	int GetSheetY() const;
	double GetSpriteScaler() const;

	Texture* GetTexture();
	bool isCreated();

private:
	Sprite(const Sprite& sprite);
	Sprite& operator=(const Sprite& sprite);

protected:
	Texture* m_pTexture;
	int m_x;
	int m_y;

	bool rotateNull;
	float m_angle;

	int m_width;
	int m_height;

	//Dynamic sprites
	int sheetX;
	int sheetY;
	double spriteScaler;

public:
	//Flip for special cases of rendering
	SDL_RendererFlip flipSprite;
};

#endif // __SPRITE_H__
