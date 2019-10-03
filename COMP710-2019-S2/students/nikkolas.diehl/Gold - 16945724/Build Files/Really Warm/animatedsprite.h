#ifndef __ANIMATEDSPRITE_H__
#define __ANIMATEDSPRITE_H__

#include "sprite.h"
#include <vector>

class BackBuffer;
class Texture;

class AnimatedSprite : public Sprite
{
	// Methods:
public:
	AnimatedSprite();
	~AnimatedSprite();

	bool Initialise(Texture& texture);
	void AddFrame(int x);
	void SetCurrentPanel(int _i);

	void Process(float deltaTime);
	void Draw(BackBuffer& backbuffer, unsigned char alpha);

	void SetFrameSpeed(float f);
	void SetFrameWidth(int w);
	void SetFrameHeight(int h);

	int GetFrameWidth();
	int GetFrameHeight();

	void Pause();
	bool IsPaused();

	bool IsAnimating();
	void StartAnimating();

	bool IsLooping();
	void SetLooping(bool b);

	int GetFrameCoords();
	int GetCurrentPanel();

protected:
	//Container for frame coordinates.
	std::vector<int> frameCoordinates;

	float m_frameSpeed;
	int m_frameWidth;
	int m_frameHeight;

	float m_timeElapsed;
	int m_currentFrame;
	int m_currentPanel;

	bool m_paused;
	bool m_loop;
	bool m_animating;

private:

};

#endif // __ANIMATEDSPRITE_H__
