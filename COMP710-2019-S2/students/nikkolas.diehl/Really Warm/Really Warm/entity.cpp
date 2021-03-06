#define _CRTDBG_MAP_ALLOC
#include "entity.h"

Entity::Entity()
: mo_sprite(0)
, entityBox(0)
, linearVelocity(0)
, entityAngle(0.0)
, entityAngleRad(0.0)
, anglularVelocity(0.0)
, force(0)
, mat()
{
	mat = new Material();
}
Entity::~Entity() {
	delete mat;
	mat = nullptr;

	delete mo_sprite;
	mo_sprite = nullptr;
}

//intialise entity with sprite
bool
Entity::Initialise(Sprite* sprite) {
	assert(sprite);
	mo_sprite = sprite;

	return (true);
}

//Set the position of the entity at x, y
//This moves the entire collision box
void
Entity::SetPos(double x, double y) {
	entityBox->X(x);
	entityBox->Y(y);
}

//Set the size of the entity with w, h
//This changes the width and height of the collision box and thus the size of the entity
void
Entity::SetSize(double w, double h) {
	entityBox->W(w);
	entityBox->H(h);

	entityBox->Center()->x = entityBox->W() / 2;
	entityBox->Center()->y = entityBox->H() / 2;
}

//Is the current entity dead
bool
Entity::IsDead()
{
	return mat->dead;
}

//Kill the current entity
void
Entity::Kill()
{
	mat->dead = true;
}

//Return the collision box and thus the entity shape
BoundingBox* 
Entity::GetCollisionBox() {
	return entityBox;
}

//Return the current position from the collision box
Vector2*
Entity::GetPos() {
	return entityBox->GetPos();
}

//Return the velocity vector
Vector2*
Entity::GetVelocity() {
	return linearVelocity;
}

double 
Entity::GetAngularVelocity() const
{
	return anglularVelocity;
}

//Return the current force vector
Vector2*
Entity::GetForce() {
	return force;
}

double* 
Entity::GetAngularVelocity()
{
	return &anglularVelocity;
}

double*
Entity::GetAngle()
{
	return &entityAngle;
}

double* 
Entity::GetRadAngle()
{
	return &entityAngleRad;
}

//Calculate the moment of intertia
void
Entity::CalculateBoxIntertia() 
{
	double w = entityBox->W();
	double h = entityBox->H();
	mat->momentOfIntertia = mat->mass * (w * w + h * h) / 12.0;
}

//Return the sprite object of the entity
Sprite* 
Entity::GetSprite() {
	return mo_sprite;
}

//Return the material of the entity
Material*
Entity::GetMat() {
	return mat;
}

//Process the given entity
void 
Entity::Process(float deltaTime) {

}

//Draw the given entity
void 
Entity::Draw(BackBuffer& backBuffer, unsigned char alpha) {
	assert(mo_sprite);

	//Set angle of sprite
	mo_sprite->SetAngle(entityAngle);

	mo_sprite->SetX(static_cast<int>(entityBox->X()));
	mo_sprite->SetY(static_cast<int>(entityBox->Y()));
	mo_sprite->Draw(backBuffer, alpha);
}