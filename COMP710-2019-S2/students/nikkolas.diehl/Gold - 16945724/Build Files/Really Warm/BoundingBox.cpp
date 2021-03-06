#define _CRTDBG_MAP_ALLOC
#include "BoundingBox.h"

#define min(a, b) (((a) < (b)) ? (a) : (b))
#define max(a, b) (((a) > (b)) ? (a) : (b))

BoundingBox::BoundingBox()
: center()
, leftMax()
, leftMin()
, rightMax()
, rightMin()
, overlap()
, collisionNormal()
{
	//Set up center
	center = std::make_unique<Vector2>();

	//Collision surfaces
	leftMax = std::make_unique<Vector2>();
	leftMin = std::make_unique<Vector2>();
	rightMax = std::make_unique<Vector2>();
	rightMin = std::make_unique<Vector2>();

	//Collision normal
	collisionNormal = std::make_unique<Vector2>();

	//Setup overlap
	overlap = new CollisionOverlap();
}
BoundingBox::BoundingBox(double _x, double _y, double _w, double _h)
: center()
, leftMax()
, leftMin()
, rightMax()
, rightMin()
, overlap()
, collisionNormal()
{
	//Set up center
	center = std::make_unique<Vector2>();

	//Collision surfaces
	leftMax = std::make_unique<Vector2>();
	leftMin = std::make_unique<Vector2>();
	rightMax = std::make_unique<Vector2>();
	rightMin = std::make_unique<Vector2>();

	//Collision normal
	collisionNormal = std::make_unique<Vector2>();

	point->Set(_x, _y);
	center->Set(_x + _w / 2, _y + _h / 2);
	mx_i_Width = _w;
	mx_i_Height = _h;
	//Setup overlap
	overlap = new CollisionOverlap();
}
BoundingBox::~BoundingBox()
{
	leftMax = nullptr;
	leftMin = nullptr;
	rightMax = nullptr;
	rightMin = nullptr;
	delete overlap;
	overlap = nullptr;
	collisionNormal = nullptr;
	point = nullptr;
	center = nullptr;
}

//Calculate the corners of the bounding box
void
BoundingBox::CalculateSides()
{
	leftMin->x = point->x; /*||*/ leftMin->y = point->y;
	leftMax->x = point->x; /*||*/ leftMax->y = point->y + mx_i_Height;
	rightMax->x = point->x + mx_i_Width; /*||*/ rightMax->y = point->y + mx_i_Height;
	rightMin->x = point->x + mx_i_Width; /*||*/ rightMin->y = point->y;
}

//Does this bounding box contain other bounding box
bool
BoundingBox::CheckCollision(BoundingBox* other) {
	//Penetration allowance in pixels
	double penetrationAllowance = 0;

	if (!Overlaps(other)) {
		return false;
	}

	Intersction(other);
	CalculateCollisionNormal(other);

	//If none of the sides of the primary rectangle are outside of other
	//Then there must be a collision of some sort
	return true;
}

void
BoundingBox::Intersction(BoundingBox* other)
{
	CalculateSides();
	other->CalculateSides();

	std::unique_ptr<Vector2> intMin = std::make_unique<Vector2>();
	std::unique_ptr<Vector2> intMax = std::make_unique<Vector2>();
	intMin->Copy(&(*leftMin));
	intMax->Copy(&(*rightMax));

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

//Calculate the collision normal
void
BoundingBox::CalculateCollisionNormal(BoundingBox* other)
{
	//Size of overlap is w, h
	//x = w || y = h

	double ax = fabs(overlap->w);
	double ay = fabs(overlap->h);

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
BoundingBox::UpdateCenter()
{
	center->x = X() + mx_i_Width / 2;
	center->y = Y() + mx_i_Height / 2;
}

//Return the collision normal
Vector2*
BoundingBox::GetNormal()
{
	return &(*collisionNormal);
}

//Return the center of the object
Vector2*
BoundingBox::Center() {
	return &(*center);
}

//Return the minimum vector
Vector2*
BoundingBox::GetMin()
{
	CalculateSides();

	return &(*leftMin);
}
//Return the maximum vector
Vector2*
BoundingBox::GetMax()
{
	CalculateSides();

	return &(*rightMax);
}
