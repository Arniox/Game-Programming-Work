#ifndef __CALC_H__
#define __CALC_H__

//Libraries
#include <random>

class Calc
{
public:
	Calc();
	~Calc();

	//Math functions
	static double constrain(double n, double low, double high);
	static double lerp(double start, double stop, double amt);
	static double map(double n, double start1, double stop1, double start2, double stop2);
	static double map(double n, double start1, double stop1, double start2, double stop2, bool winthinBounds);
	static double max(double a, double b);
	static double min(double a, double b);
	static double RadToDeg(double a);
	static double DegToRad(double a);
	static float RadToDegf(float a);
	static float DegToRadf(float a);
	static double Random(double LO, double HI);
	static bool WithinBounds(int posX, int posY, int x1, int y1, int x2, int y2);
	static double constrainAngle360(double x);
	static double constrainAngle180(double x);
	static double angleDiff(double a, double b);
	static double bisectAngle360(double a, double b);
	static double bisectAngle180(double a, double b);
};

#endif