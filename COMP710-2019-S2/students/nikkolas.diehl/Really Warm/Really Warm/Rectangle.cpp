#include "Rectangle.h"

#define min(a, b) (((a) < (b)) ? (a) : (b))
#define max(a, b) (((a) > (b)) ? (a) : (b))

Rectangle::Rectangle()
: mx_i_Width(0)
, mx_i_Height(0)
, point(0)
{
	point = new Vector2();
}
Rectangle::Rectangle(double _x, double _y, double _w, double _h)
: mx_i_Width(0)
, mx_i_Height(0)
, point(0)
{
	point = new Vector2(_x, _y);
	mx_i_Width = _w;
	mx_i_Height = _h;
}
Rectangle::~Rectangle()
{
	delete point;
	point = 0;
}

//Does this rectangle contain a given vector2
bool
Rectangle::Contains(Vector2* otherPoint) const{
	return (otherPoint->x > point->x - mx_i_Width &&
			otherPoint->x < point->x + mx_i_Width &&
			otherPoint->y > point->y - mx_i_Height &&
			otherPoint->y < point->y + mx_i_Height);
}

//Get position of the rectangle
Vector2*
Rectangle::GetPos() {
	return point;
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