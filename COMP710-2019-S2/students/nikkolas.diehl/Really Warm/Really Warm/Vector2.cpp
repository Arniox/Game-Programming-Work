#include "Vector2.h"

//Static variables
Vector2* Vector2::mp_Instance = 0;

Vector2&
Vector2::CreateVector(float in_x, float in_y) {
	//Create and return
	if (mp_Instance == 0) {
		mp_Instance = new Vector2();

		mp_Instance->x = in_x;
		mp_Instance->y = in_y;
	}

	return (*mp_Instance);
}

Vector2::Vector2()
{
}

Vector2::~Vector2()
{
}


//----------------------Operator Overrides----------------------

bool
Vector2::operator==(Vector2& other) const 
{
	const float _inaccuracy = 0.00001f; //Floating points compared with 1e-5

	return ((x > other.x - _inaccuracy && x < other.x + _inaccuracy) &&
		    (y > other.y - _inaccuracy && y < other.y + _inaccuracy));
}

bool
Vector2::operator!=(Vector2& other) const 
{
	return (!(*this == other));
}

bool 
Vector2::operator<(Vector2& other) const
{
	return (x < other.x && y < other.y);
}

bool
Vector2::operator>(Vector2& other) const
{
	return (x > other.x && y > other.y);
}

Vector2& 
Vector2::operator-(Vector2& other) 
{
	return Vector2::CreateVector((x - other.x), (y - other.y));
}

Vector2& 
Vector2::operator+(Vector2& other) 
{
	return Vector2::CreateVector((x + other.x), (y + other.y));
}


//----------------------Functionalities----------------------
float
Vector2::Magnatude() {
	return static_cast<float>(sqrt(pow(x, 2) + pow(y, 2)));
}

float 
Vector2::Dot(Vector2& other) {
	return (x * other.x + y * other.y);
}

float
Vector2::Cross2D(Vector2& other_p1, Vector2& other_p2) {
	Vector2 Uc = (*this) - other_p1;
	Vector2 Vc = other_p2 - other_p1;

	return Uc.x * Vc.y - Uc.y * Vc.x;
}

float
Vector2::AngleRad(Vector2& other) {
	return atan2(other.y - y, other.x - x);
}

float
Vector2::AngleDeg(Vector2& other) {
	return static_cast<float>(AngleRad(other) * 180.0f * M_PI);
}

Vector2&
Vector2::Scale(float& scale) {
	return Vector2::CreateVector((x * scale), (y * scale));
}

Vector2&
Vector2::Normalized() {
	return Vector2::CreateVector((x / this->Magnatude()), y / this->Magnatude());
}

