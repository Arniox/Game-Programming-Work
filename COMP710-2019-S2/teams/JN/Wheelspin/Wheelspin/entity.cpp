#define _CRTDBG_MAP_ALLOC

//Locals
#include "entity.h"
#include "sprite.h"
#include "backbuffer.h"


Entity::Entity()
: m_pSprite(0)
{
}
Entity::~Entity() 
{
	delete m_pSprite;
	m_pSprite = nullptr;
}

//intialise entity with sprite
bool
Entity::Initialise(Sprite* sprite) {
	assert(sprite);
	m_pSprite = sprite;

	return (true);
}

//Set the position of the entity at x, y
//This moves the entire collision box
void
Entity::SetPos(double x, double y) 
{
	
}

//Set the size of the entity with w, h
//This changes the width and height of the collision box and thus the size of the entity
void
Entity::SetSize(double w, double h) 
{
	
}

//Is the current entity dead
bool
Entity::IsDead()
{
	return false;
}

//Kill the current entity
void
Entity::Kill()
{
	
}

//Return the sprite object of the entity
Sprite*
Entity::GetSprite() 
{
	return m_pSprite;
}

//Process the given entity
void
Entity::Process(float deltaTime) 
{

}

//Draw the given entity
void
Entity::Draw(BackBuffer& backBuffer, unsigned char alpha) {
	assert(m_pSprite);

	//Set angle of sprite
	m_pSprite->SetAngle(m_entityAngle);

	m_pSprite->SetX(0);
	m_pSprite->SetY(0);
	m_pSprite->Draw(backBuffer, alpha);
}