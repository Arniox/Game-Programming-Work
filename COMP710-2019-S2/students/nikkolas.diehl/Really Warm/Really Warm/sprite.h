// COMP710 GP 2D Framework 2019
#ifndef __SPRITE_H__
#define __SPRITE_H__

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
	void Draw(BackBuffer& backbuffer);

	void SetX(int x);
	void SetY(int y);
	void RotateNull(bool rotate);
	bool GetRotate();

	int GetX() const;
	int GetY() const;

	void SetAngle(double angle);
	float GetAngle() const;

	void SetLength(int legnth);

	void SetCenter(int x, int y);
	void SetHandleCenter();

	int GetWidth() const;
	int GetHeight() const;

	Texture* GetTexture();

private:
	Sprite(const Sprite& sprite);
	Sprite& operator=(const Sprite& sprite);

protected:
	Texture* m_pTexture;
	int m_x;
	int m_y;

	bool rotateNull;
	float m_angle;
	int m_centerX;
	int m_centerY;

	int m_width;
	int m_height;

private:

};

#endif // __SPRITE_H__
