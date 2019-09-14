#pragma once

#include "Vector2.h"
#include <SDL.h>

class Rectangle
{
public:
	Rectangle();
	Rectangle(double _x, double _y, double _w, double _h);
	~Rectangle();

	//Return vector
	Vector2* GetPos();

	//Return data
	double& X();
	double& Y();
	double& W();
	double& H();
	//Set data
	void X(double _x);
	void Y(double _x);
	void W(double _x);
	void H(double _x);

	//Contains
	bool Contains(Vector2* point) const;

protected:
	Vector2* point; //top left corner
	double mx_i_Width;
	double mx_i_Height;
};

