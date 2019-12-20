#define _CRTDBG_MAP_ALLOC

//Locals
#include "Calc.h"
// Constants already contains an import for math, removed redundant import causing warnings - Jordan
// Remember, all includes as part of a header file are automatically included for any file that includes that
#include "Constants.h"

Calc::Calc()
{
}
Calc::~Calc()
{
}

//Constrain a value "n" within a low to high range
double 
Calc::constrain(double n, double low, double high)
{
	return max(min(n, high), low);
}

//Lerp a value "amt" from start to stop
double 
Calc::lerp(double start, double stop, double amt)
{
	return amt * (stop - start) + start;
}

//Map a value "n" with a given range of start1 - stop1 to another range of start2 stop2
//Say a value 0.0 - 5.0 can be mapped to 10 - 670
double 
Calc::map(double n, double start1, double stop1, double start2, double stop2)
{
	return (n - start1) / (stop1 - start1) * (stop2 - start2) + start2;
}

//Map within the bounds of start2 to stop2 given an optional bool for map function
double 
Calc::map(double n, double start1, double stop1, double start2, double stop2, bool winthinBounds)
{
	double newVal = map(n, start1, stop1, start2, stop2);
	if (start2 < stop2) 
	{
		return constrain(newVal, start2, stop2);
	}
	else {
		return constrain(newVal, stop2, start2);
	}
}

//Return the maximum of a and b
double 
Calc::max(double a, double b)
{
	return ((a) > (b) ? (a) : (b));
}

//Return the minamum of a and b
double 
Calc::min(double a, double b)
{
	return ((a) < (b) ? (a) : (b));
}

double 
Calc::RadToDeg(double a)
{
	return a * 180.0 / PI;
}

double 
Calc::DegToRad(double a)
{
	return a * PI / 180.0f;
}

float 
Calc::RadToDegf(float a)
{
	return a * 180.0f / static_cast<float>(PI);
}

float 
Calc::DegToRadf(float a)
{
	return a * static_cast<float>(PI) / 180.0f;
}

double
Calc::Random(double LO, double HI)
{
	
	return LO + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (HI - LO)));
}

bool 
Calc::WithinBounds(int posX, int posY, int x1, int y1, int x2, int y2)
{
	return (posX > x1 - CHECK_INACCURACY && posX < x2 + CHECK_INACCURACY && 
			posY > y1 - CHECK_INACCURACY && posY < y2 + CHECK_INACCURACY);
}

double 
Calc::constrainAngle360(double x)
{
	double deg = x;

	deg -= 360. * floor(deg * (1. / 360.));
	return deg;
}

double 
Calc::constrainAngle180(double x)
{
	double deg = x;

	deg -= 360. * floor((deg + 180.) * (1. / 360.));
	return deg;
}

double 
Calc::angleDiff(double a, double b)
{
	double dif = fmod(b - a + 180, 360);
	if (dif < 0)
		dif += 360;
	return dif - 180;
}

double 
Calc::bisectAngle360(double a, double b)
{
	return constrainAngle360(a + angleDiff(a, b) * 0.5);
}

double
Calc::bisectAngle180(double a, double b)
{
	return constrainAngle180(a + angleDiff(a, b) * 0.5);
}
