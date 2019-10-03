#ifndef __RECTANGLE_H__
#define __RECTANGLE_H__

#include "Vector2.h"
#include <SDL.h>

class Rectangle
{
public:
	Rectangle();
	Rectangle(double _x, double _y, double _w, double _h);
	void Set(double _x, double _y, double _w, double _h);
	~Rectangle();

	std::unique_ptr<Rectangle> IncreaseSize(int sizeInc);

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
	//Intersects
	bool Overlaps(Rectangle* other);

protected:
	std::unique_ptr<Vector2> point = std::make_unique<Vector2>(); //top left corner
	double mx_i_Width;
	double mx_i_Height;
};

#endif

