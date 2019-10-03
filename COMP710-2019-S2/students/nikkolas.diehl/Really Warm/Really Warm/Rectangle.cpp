#define _CRTDBG_MAP_ALLOC
#include "Rectangle.h"

#define min(a, b) (((a) < (b)) ? (a) : (b))
#define max(a, b) (((a) > (b)) ? (a) : (b))

Rectangle::Rectangle()
: mx_i_Width(0)
, mx_i_Height(0)
, point()
{
	point = std::make_unique<Vector2>();
}
Rectangle::Rectangle(double _x, double _y, double _w, double _h)
: mx_i_Width(0)
, mx_i_Height(0)
, point()
{
	point = std::make_unique<Vector2>();
	point->Set(_x, _y);
	mx_i_Width = _w;
	mx_i_Height = _h;
}
void 
Rectangle::Set(double _x, double _y, double _w, double _h)
{
	X(_x);
	Y(_y);
	W(_w);
	H(_h);
}
Rectangle::~Rectangle()
{
	point = nullptr;
}

std::unique_ptr<Rectangle> 
Rectangle::IncreaseSize(int sizeInc)
{
	std::unique_ptr<Rectangle> smart_size = std::make_unique<Rectangle>();

	smart_size->X(X() - sizeInc);
	smart_size->Y(Y() - sizeInc);
	smart_size->W(W() + sizeInc * 2);
	smart_size->H(H() + sizeInc * 2);

	return smart_size;
}

//Does this rectangle contain a given vector2
bool
Rectangle::Contains(Vector2* otherPoint) const{
	return (otherPoint->x > point->x &&
			otherPoint->x < point->x + mx_i_Width &&
			otherPoint->y > point->y &&
			otherPoint->y < point->y + mx_i_Height);
}

bool 
Rectangle::Overlaps(Rectangle * other)
{
	//Penetration allowance in pixels
	double penetrationAllowance = 0;

	//The sides of the rectangles
	double leftA, leftB;
	double rightA, rightB;
	double topA, topB;
	double bottomA, bottomB;

	//Calculate the sides of rectangle A
	leftA = this->X() + penetrationAllowance;
	rightA = this->X() + (this->W() - penetrationAllowance);
	topA = this->Y() + penetrationAllowance;
	bottomA = this->Y() + (this->H() - penetrationAllowance);

	//Calculate the sides of rectangle B
	leftB = other->X() + penetrationAllowance;
	rightB = other->X() + (other->W() - penetrationAllowance);
	topB = other->Y() + penetrationAllowance;
	bottomB = other->Y() + (other->H() - penetrationAllowance);

	//If any of the sides from rectangle A are outside of B
	//Then there is no possible way the two boxes are colliding
	if (bottomA <= topB) {
		return false;
	}
	if (topA >= bottomB) {
		return false;
	}
	if (rightA <= leftB) {
		return false;
	}
	if (leftA >= rightB) {
		return false;
	}

	//Otherwise overlap is true
	return true;
}

//Get position of the rectangle
Vector2*
Rectangle::GetPos() {
	return &(*point);
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