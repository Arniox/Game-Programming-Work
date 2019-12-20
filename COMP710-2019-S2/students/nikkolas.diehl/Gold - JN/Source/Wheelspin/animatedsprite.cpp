#define _CRTDBG_MAP_ALLOC

// This includes:
#include "animatedsprite.h"

// Local includes:
#include "texture.h"
#include "backbuffer.h"

AnimatedSprite::AnimatedSprite()
: m_frameSpeed(0.0f)
, m_frameWidth(0)
, m_frameHeight(0)
, m_timeElapsed(0.0f)
, m_currentFrame(0)
, m_currentPanel(0)
, m_paused(false)
, m_loop(false)
, m_animating(false)
{
}
AnimatedSprite::~AnimatedSprite()
{
}

bool
AnimatedSprite::Initialise(Texture& texture)
{
	m_frameWidth = 0;
	m_frameHeight = 0;
	m_frameSpeed = 0;
	m_currentFrame = 0;
	m_currentPanel = 0;

	m_loop = false;
	m_paused = false;
	m_animating = true;

	Sprite::Initialise(texture);

	StartAnimating();

	return (true);
}

void
AnimatedSprite::AddFrame(int x)
{
	//Add the x coordinate to the frame coordinate container.
	frameCoordinates.push_back(x);
}

void
AnimatedSprite::SetCurrentPanel(int _i)
{
	m_currentPanel = _i;
}

void
AnimatedSprite::Process(float deltaTime)
{
	//If not paused...
	if (!m_paused) {
		if (m_animating) {
			//Count the time elapsed.
			m_timeElapsed++;

			//If the time elapsed is greater than the frame speed move to the next frame
			if (m_timeElapsed > m_frameSpeed) {
				m_currentFrame++;
				m_timeElapsed = 0;
			}

			//If the current frame is greater than the number of frames in this animation
			if (m_currentFrame >= (signed int)frameCoordinates.size()) {
				m_currentFrame = 0;
				//m_animating = false;
			}

			//Change frame
			m_x = frameCoordinates.at(m_currentFrame);
		}
	}
}

void
AnimatedSprite::Draw(BackBuffer& backbuffer, unsigned char alpha)
{
	Draw(backbuffer, alpha, false);
}

void 
AnimatedSprite::Draw(BackBuffer& backBuffer, unsigned char alpha, bool HUD)
{
	backBuffer.DrawAnimatedSprite(*this, alpha, HUD);
}

void
AnimatedSprite::SetFrameSpeed(float f)
{
	m_frameSpeed = f;
}

void
AnimatedSprite::SetFrameWidth(int w)
{
	m_frameWidth = w;
}

void
AnimatedSprite::SetFrameHeight(int h)
{
	m_frameHeight = h;
}

int 
AnimatedSprite::GetFrameCoords()
{
	return frameCoordinates.at(m_currentFrame);
}

int
AnimatedSprite::GetCurrentPanel()
{
	return m_currentPanel * m_frameHeight;
}

int 
AnimatedSprite::GetFrameWidth()
{
	return m_frameWidth;
}

int
AnimatedSprite::GetFrameHeight()
{
	return m_frameHeight;
}

void
AnimatedSprite::Pause()
{
	m_paused = !m_paused;
}

bool
AnimatedSprite::IsPaused()
{
	return (m_paused);
}

bool
AnimatedSprite::IsAnimating()
{
	return (m_animating);
}

void
AnimatedSprite::StartAnimating()
{
	m_animating = true;

	m_timeElapsed = 0;
	m_currentFrame = 0;
}

bool
AnimatedSprite::IsLooping()
{
	return (m_loop);
}

void
AnimatedSprite::SetLooping(bool b)
{
	m_loop = b;
}

