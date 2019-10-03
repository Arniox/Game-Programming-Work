#define _CRTDBG_MAP_ALLOC
// COMP710 GP 2D Framework 2019

// This include:
#include "sprite.h"

// Local includes:
#include "backbuffer.h"
#include "texture.h"

Sprite::Sprite()
: m_pTexture(0)
, m_x(0)
, m_y(0)
, m_width(0)
, m_height(0)
, m_angle(0.0f)
, rotateNull(true)
, flipSprite(SDL_FLIP_NONE)
, spriteScaler(1)
, sheetX(0)
, sheetY(0)
{
}

Sprite::~Sprite()
{
}

bool 
Sprite::Initialise(Texture& texture)
{
	m_pTexture = &texture;
	
	m_width = m_pTexture->GetWidth();
	m_height = m_pTexture->GetHeight();

	return (true);
}

void 
Sprite::Process(float deltaTime)
{

}

void 
Sprite::Draw(BackBuffer& backbuffer, unsigned char alpha)
{
	backbuffer.DrawSprite(*this, alpha);
}

void
Sprite::RotateNull(bool rotate)
{
	rotateNull = rotate;
}

bool
Sprite::GetRotate()
{
	return rotateNull;
}

void 
Sprite::SetX(int x)
{
	m_x = x;
}

void 
Sprite::SetY(int y)
{
	m_y = y;
}

int
Sprite::GetX() const
{
	return (m_x);
}

int
Sprite::GetY() const
{
	return (m_y);
}

void
Sprite::SetAngle(double angle)
{
	m_angle = static_cast<float>(angle);
}

float
Sprite::GetAngle() const
{
	return (m_angle);
}

//overwrite the sprite length (width) for custom length
void
Sprite::SetLength(int length) {
	m_width = length;
}

//overwrite the sprite height for a custom height
void
Sprite::SetHeight(int height)
{
	m_height = height;
}

Texture*
Sprite::GetTexture()
{
	return m_pTexture;
}

bool 
Sprite::isCreated()
{
	return (this != NULL ? true : false);
}

int 
Sprite::GetWidth() const
{
	return (m_width);
}

int
Sprite::GetHeight() const
{
	return (m_height);
}

void Sprite::SetSheetX(int _x)
{
	sheetX = _x;
}

void Sprite::SetSheetY(int _y)
{
	sheetY = _y;
}

void Sprite::SetSpriteScaler(double _scaler)
{
	spriteScaler = _scaler;
}

int Sprite::GetSheetX() const
{
	return sheetX;
}

int Sprite::GetSheetY() const
{
	return sheetY;
}

double Sprite::GetSpriteScaler() const
{
	return spriteScaler;
}
