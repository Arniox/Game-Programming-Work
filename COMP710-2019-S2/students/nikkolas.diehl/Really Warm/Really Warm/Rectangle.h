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
	Vector2* Center();
	
	//Return shape data
	Vector2* GetMin();
	Vector2* GetMax();

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
	//Collides
	bool CheckCollision(Rectangle* other);
	//Calculate the overlaping box
	void Intersction(Rectangle* other);
	//Calculate the sides of the rectangle
	void CalculateSides();
	//Reconstruct the overlap and set to 0
	void Reconstruct();
	//Return size of overlap
	std::unique_ptr<Vector2> OverLapSize();
	//Calculate the collision normal
	void CalculateCollisionNormal(Rectangle* other);
	//Get the collision normal
	Vector2* GetNormal();
	void UpdateCenter();

private:
	//Collision surfaces
	Vector2* leftMax;
	Vector2* leftMin;
	Vector2* rightMax;
	Vector2* rightMin;
	//Collision overlap
	struct CollisionOverlap {
		double x = 0;
		double y = 0;
		double w = 0;
		double h = 0;
	};
	CollisionOverlap* overlap;
	//Collision normal
	Vector2* collisionNormal;

private:
	Vector2* point; //top left corner
	Vector2* center; //center
	double mx_i_Width;
	double mx_i_Height;
};

