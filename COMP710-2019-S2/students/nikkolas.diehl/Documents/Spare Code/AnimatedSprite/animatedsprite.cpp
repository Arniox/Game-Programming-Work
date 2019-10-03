// COMP710 GP 2D Framework 2019

// This includes:
#include "animatedsprite.h"

// Local includes:
#include "texture.h"
#include "backbuffer.h"

AnimatedSprite::AnimatedSprite()
: m_frameSpeed(0.0f)
, m_frameWidth(0)
, m_timeElapsed(0.0f)
, m_currentFrame(0)
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
	m_frameSpeed = 0;

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
	// SS05.5: Add the x coordinate to the frame coordinate container.
}

void 
AnimatedSprite::Process(float deltaTime)
{
	// SS05.5: If not paused...
	// SS05.5: Count the time elapsed.

	// SS05.5: If the time elapsed is greater than the frame speed.

	// SS05.5: Move to the next frame.
	// SS05.5: Reset the time elapsed counter.

	// SS05.5: If the current frame is greater than the number 
	//          of frame in this animation...
	// SS05.5: Reset to the first frame.

	// SS05.5: Stop the animation if it is not looping...
}

void 
AnimatedSprite::Draw(BackBuffer& backbuffer)
{
	// SS05.5: Draw the particular frame into the backbuffer.

	//          What is the current frame's x coordinate?
	//          What is the frame width?
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