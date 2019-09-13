#include "Rectangle.h"

#define min(a, b) (((a) < (b)) ? (a) : (b))
#define max(a, b) (((a) > (b)) ? (a) : (b))

Rectangle::Rectangle()
: mx_i_Width(0)
, mx_i_Height(0)
, point(0)
, center(0)
, leftMax()
, leftMin()
, rightMax()
, rightMin()
, overlap()
, collisionNormal()
{
	point = new Vector2();
	center = new Vector2();
	//Setup overlap
	overlap = new CollisionOverlap();
	//Set up sides
	leftMin = new Vector2();
	leftMax = new Vector2();
	rightMax = new Vector2();
	rightMin = new Vector2();
	//Set up collision normal
	collisionNormal = new Vector2();
}
Rectangle::Rectangle(double _x, double _y, double _w, double _h)
: mx_i_Width(0)
, mx_i_Height(0)
, point(0)
, leftMax()
, leftMin()
, rightMax()
, rightMin()
, collisionNormal()
{
	point = new Vector2(_x, _y);
	center = new Vector2(_x + _w / 2, _y + _h / 2);
	mx_i_Width = _w;
	mx_i_Height = _h;
	//Setup overlap
	overlap = new CollisionOverlap();
	//Set up sides
	leftMin = new Vector2();
	leftMax = new Vector2();
	rightMax = new Vector2();
	rightMin = new Vector2();
	//Set up collision normal
	collisionNormal = new Vector2();
}
Rectangle::~Rectangle()
{
	delete point;
	point = 0;

	delete leftMax;
	leftMax = 0;
	delete leftMin;
	leftMin = 0;
	delete rightMax;
	rightMax = 0;
	delete rightMin;
	rightMin = 0;
	delete overlap;
	overlap = 0;
	delete collisionNormal;
	collisionNormal = 0;
}

void
Rectangle::CalculateSides()
{
	leftMin->x = point->x; /*||*/ leftMin->y = point->y;
	leftMax->x = point->x; /*||*/ leftMax->y = point->y + mx_i_Height;
	rightMax->x = point->x + mx_i_Width; /*||*/ rightMax->y = point->y + mx_i_Height;
	rightMin->x = point->x; /*||*/ rightMin->y = mx_i_Width, point->y;
}

//Does this rectangle contain a given vector2
bool
Rectangle::Contains(Vector2* otherPoint) const{
	return (otherPoint->x > point->x - mx_i_Width &&
			otherPoint->x < point->x + mx_i_Width &&
			otherPoint->y > point->y - mx_i_Height &&
			otherPoint->y < point->y + mx_i_Height);
}

//Does this rectangle contain other rectangle
bool
Rectangle::CheckCollision(Rectangle* other) {
	//Penetration allowance in pixels
	double penetrationAllowance = 0;

	//The sides of the rectangles
	int leftA, leftB;
	int rightA, rightB;
	int topA, topB;
	int bottomA, bottomB;

	//Calculate the sides of rectangle A
	leftA   = static_cast<int>(this->X() + penetrationAllowance);
	rightA  = static_cast<int>(this->X() + (this->W() - penetrationAllowance));
	topA    = static_cast<int>(this->Y() + penetrationAllowance);
	bottomA = static_cast<int>(this->Y() + (this->H() - penetrationAllowance));

	//Calculate the sides of rectangle B
	leftB   = static_cast<int>(other->X() + penetrationAllowance);
	rightB  = static_cast<int>(other->X() + (other->W() - penetrationAllowance));
	topB    = static_cast<int>(other->Y() + penetrationAllowance);
	bottomB = static_cast<int>(other->Y() + (other->H() - penetrationAllowance));

	//If any of the sides from rectangle A are outside of B
	//Then there is no possible way the two boxes are colliding
	if (bottomA <= topB) {
		Reconstruct();
		return false;
	}
	if (topA >= bottomB) {
		Reconstruct();
		return false;
	}
	if (rightA <= leftB) {
		Reconstruct();
		return false;
	}
	if (leftA >= rightB) {
		Reconstruct();
		return false;
	}

	Intersction(other);
	CalculateCollisionNormal(other);

	//If none of the sides of the primary rectangle are outside of other
	//Then there must be a collision of some sort
	return true;
}

void
Rectangle::Intersction(Rectangle* other)
{
	CalculateSides();
	other->CalculateSides();

	std::unique_ptr<Vector2> intMin = std::make_unique<Vector2>();
	std::unique_ptr<Vector2> intMax = std::make_unique<Vector2>();
	intMin->Copy(leftMin);
	intMax->Copy(rightMax);

	intMin->MakeCeil(other->GetMin());
	intMax->MakeFloor(other->GetMax());

	//Check intersection isn't null
	if (intMin->x < intMax->x && intMin->y < intMax->y) {
		double width = intMax->x - intMin->x;
		double height = intMax->y - intMin->y;

		overlap->x = static_cast<int>(intMin->x);
		overlap->y = static_cast<int>(intMin->y);
		overlap->w = static_cast<int>(width);
		overlap->h = static_cast<int>(height);
	}
}

//Reconstruct the rectangle with everything at 0
void
Rectangle::Reconstruct()
{
	overlap->x = 0; overlap->y = 0; overlap->w = 0; overlap->h = 0;
	collisionNormal->x = 0; collisionNormal->y = 0;
}

//Return the size of an overlap
std::unique_ptr<Vector2>
Rectangle::OverLapSize()
{
	std::unique_ptr<Vector2> smart_size = std::make_unique<Vector2>();
	(*smart_size).x = overlap->w;
	(*smart_size).y = overlap->h;

	return smart_size;
}

//Calculate the collision normal
void
Rectangle::CalculateCollisionNormal(Rectangle* other) 
{
	//Get size of overlap
	std::unique_ptr<Vector2> size = std::make_unique<Vector2>();
	size = move(OverLapSize());

	double ax = fabs(size->x);
	double ay = fabs(size->y);

	//Update the central position and then calc normal
	UpdateCenter();
	other->UpdateCenter();
	double sx = (this->center->x > other->center->x ? -1.0 : 1.0);
	double sy = (this->center->y > other->center->y ? -1.0 : 1.0);

	if (ax <= ay) {
		collisionNormal->x = sx;
		collisionNormal->y = 0.0;
	}
	else {
		collisionNormal->x = 0.0;
		collisionNormal->y = sy;
	}
}

//Update the entity center position
void
Rectangle::UpdateCenter()
{
	center->x = X() + mx_i_Width / 2;
	center->y = Y() + mx_i_Height / 2;
}

//Return the collision normal
Vector2*
Rectangle::GetNormal()
{
	return collisionNormal;
}

//Get position of the rectangle
Vector2*
Rectangle::GetPos() {
	return point;
}

//Return the center of the object
Vector2*
Rectangle::Center() {
	return center;
}

//Return the minimum vector
Vector2*
Rectangle::GetMin()
{
	return leftMin;
}
//Return the maximum vector
Vector2*
Rectangle::GetMax()
{
	return rightMax;
}

//Return data
double&
Rectangle::X() {
	return point->x;
}
double&
Rectangle::Y() {
	return point->y;
}
double&
Rectangle::W() {
	return mx_i_Width;
}
double&
Rectangle::H() {
	return mx_i_Height;
}

//Set data
void
Rectangle::X(double _x) {
	point->x = _x;
}

void
Rectangle::Y(double _y) {
	point->y = _y;
}

void
Rectangle::W(double _w) {
	mx_i_Width = _w;
}

void
Rectangle::H(double _h) {
	mx_i_Height = _h;
}