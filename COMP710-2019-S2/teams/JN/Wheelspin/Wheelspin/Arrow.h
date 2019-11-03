#ifndef __ARROW_H__
#define __ARROW_H__

class BackBuffer;
class Sprite;

class Arrow
{
public:
	Arrow(int x, int y);
	~Arrow();
	void Process(float deltaTime, float x, float y);
	void Draw(BackBuffer& backBuffer);
	void SetPointTo(float x, float y);
private:
	Sprite* m_pSprite;
	float m_angle;
	int m_x;
	int m_y;
};

#endif // !__ARROW_H__