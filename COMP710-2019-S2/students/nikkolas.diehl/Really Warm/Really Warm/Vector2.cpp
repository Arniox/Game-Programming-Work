#include "Vector2.h"

// Converts degrees to radians.
#define degreesToRadians(angleDegrees) (angleDegrees * M_PI / 180.0)
// Converts radians to degrees.
#define radiansToDegrees(angleRadians) (angleRadians * 180.0 / M_PI)

Vector2::Vector2()
: x(0.0)
, y(0.0)
{
}
Vector2::Vector2(double in_x, double in_y)
: x(0.0)
, y(0.0)
{
	x = in_x;
	y = in_y;
}
Vector2::~Vector2()
{
}

//----------------------Operator Overrides----------------------
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

bool Vector2::operator<=(Vector2 & other) const
{
	return (x <= other.x && y <= other.y);
}

bool Vector2::operator>=(Vector2 & other) const
{
	return (x >= other.x && y >= other.y);
}

std::unique_ptr<Vector2>
Vector2::operator-(Vector2& other)
{
	//Create smart pointer
	std::unique_ptr<Vector2> smart_minus = std::make_unique<Vector2>();
	(*smart_minus).x = x - other.x;
	(*smart_minus).y = y - other.y;

	return smart_minus;
}

std::unique_ptr<Vector2>
Vector2::operator+(Vector2& other)
{
	//Create smart pointer
	std::unique_ptr<Vector2> smart_add = std::make_unique<Vector2>();
	(*smart_add).x = x + other.x;
	(*smart_add).y = y + other.y;

	return smart_add;
}

std::unique_ptr<Vector2>
Vector2::operator*(Vector2& other)
{
	//Create smart pointer
	std::unique_ptr<Vector2> smart_times = std::make_unique<Vector2>();
	(*smart_times).x = x * other.x;
	(*smart_times).y = y * other.y;

	return smart_times;
}

std::unique_ptr<Vector2>
Vector2::operator/(Vector2& other)
{
	//Create smart pointer
	std::unique_ptr<Vector2> smart_divide = std::make_unique<Vector2>();
	(*smart_divide).x = x / other.x;
	(*smart_divide).y = y / other.y;

	return smart_divide;
}

//----------------------Vector2 Math-------------------------
void
Vector2::Add(Vector2* other) 
{
	x += other->x;
	y += other->y;
}
void
Vector2::Minus(Vector2* other)
{
	x -= other->x;
	y -= other->y;
}
void
Vector2::Times(Vector2* other) 
{
	x *= other->x;
	y *= other->y;
}
void
Vector2::Divide(Vector2* other)
{
	x /= other->x;
	y /= other->y;
}
void
Vector2::Ceil()
{
	x = ceil(x);
	y = ceil(y);
}
void
Vector2::Floor()
{
	x = floor(x);
	y = floor(y);
}
double
Vector2::Heading()
{
	return atan2(x, y);
}
void
Vector2::Clear()
{
	x = 0.0;
	y = 0.0;
}

//----------------------Extra Vector2 Math-------------------
void
Vector2::MakeCeil(Vector2* other)
{
	if (other->x > x) x = other->x;
	if (other->y > y) y = other->y;
}

void
Vector2::MakeFloor(Vector2* other)
{
	if (other->x < x) x = other->x;
	if (other->y < y) y = other->y;
}

//----------------------Scaler-------------------------------
std::unique_ptr<Vector2>
Vector2::ScaleMult(double scale)
{
	//Create smart pointer
	std::unique_ptr<Vector2> scale_mult = std::make_unique<Vector2>();
	(*scale_mult).x = x * scale;
	(*scale_mult).y = y * scale;

	return scale_mult;
}

std::unique_ptr<Vector2>
Vector2::ScaleDiv(double scale)
{
	//Create smart pointer
	std::unique_ptr<Vector2> scale_div = std::make_unique<Vector2>();
	(*scale_div).x = x / scale;
	(*scale_div).y = y / scale;

	return scale_div;
}

//----------------------Vector2 returns----------------------
std::unique_ptr<Vector2>
Vector2::Normalize() 
{
	//Create smart pointer
	std::unique_ptr<Vector2> normalize = std::make_unique<Vector2>();
	(*normalize).x = x / this->Magnatude();
	(*normalize).y = y / this->Magnatude();

	return normalize;
}
std::unique_ptr<Vector2>
Vector2::Normal() 
{
	//Create smart pointer
	std::unique_ptr<Vector2> norm = std::make_unique<Vector2>();
	(*norm).x = y;
	(*norm).y = -x;

	return norm;
}

//----------------------Functionalities----------------------
double
Vector2::Dot(Vector2* second)
{
	return (x * second->x + y * second->y);
}

void
Vector2::Copy(Vector2* from) {
	x = from->x + 0;
	y = from->y + 0;
}

double
Vector2::Magnatude() {
	return sqrt(x * x + y * y);
}

double
Vector2::AngleRad(Vector2* other) {
	return atan2(other->y - y, other->x - x);
}

double
Vector2::AngleDeg(Vector2* other) {
	return radiansToDegrees(AngleRad(other));
}

//----------------------Position Checking----------------------

double
Vector2::Distance(Vector2* other) {
	return (sqrt(pow(other->x - x, 2) + pow(other->y - y, 2)));
}


//----------------------Extra Utilities----------------------
bool
Vector2::ValuesEqual(double one, double two) {
	return((one > two - _inaccuracy) && (one < two + _inaccuracy));
}

bool
Vector2::Lerp(Vector2* towards, double speed) {
	//*this += *(towards - *this)->Scale(speed);
	x += (towards->x - x) * speed;
	y += (towards->y - y) * speed;

	return (ValuesEqual(x, towards->x) && ValuesEqual(y, towards->y));
}

bool
Vector2::LerpX(Vector2* towards, double speed) {
	x += (towards->x - x) *speed;

	return (ValuesEqual(x, towards->x));
}

bool
Vector2::LerpY(Vector2* towards, double speed) {
	y += (towards->y - y) *speed;

	return (ValuesEqual(y, towards->y));
}

double
Vector2::Max() {
	return (x > y ? x : y);
}

double
Vector2::Min() {
	return (x < y ? x : y);
}

void
Vector2::Negate() {
	x *= -1;
	y *= -1;
}

std::string
Vector2::ToString() {
	return ("( " + std::to_string(x) + ", " + std::to_string(y) + " )");
}