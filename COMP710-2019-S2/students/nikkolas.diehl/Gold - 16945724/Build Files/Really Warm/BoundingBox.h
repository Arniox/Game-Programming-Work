#ifndef __BOUNDINGBOX_H__
#define __BOUNDINGBOX_H__

#include "Rectangle.h"

class BoundingBox:
	public Rectangle
{
public:
	BoundingBox();
	BoundingBox(double _x, double _y, double _w, double _h);
	~BoundingBox();

	//Get the center of the bounding box
	Vector2* Center();

	//Return shape data
	Vector2* GetMin();
	Vector2* GetMax();

	//Collides
	bool CheckCollision(BoundingBox* other);
	//Calculate the overlaping box
	void Intersction(BoundingBox* other);
	//Calculate the sides of the rectangle
	void CalculateSides();
	//Calculate the collision normal
	void CalculateCollisionNormal(BoundingBox* other);
	//Get the collision normal
	Vector2* GetNormal();
	void UpdateCenter();

protected:
	std::unique_ptr<Vector2> center; //Center

	//Collision surfaces
	std::unique_ptr<Vector2> leftMax;
	std::unique_ptr<Vector2> leftMin;
	std::unique_ptr<Vector2> rightMax;
	std::unique_ptr<Vector2> rightMin;
	//Collision overlap
	struct CollisionOverlap {
		double x = 0;
		double y = 0;
		double w = 0;
		double h = 0;
	};
	CollisionOverlap* overlap;
	//Collision normal
	std::unique_ptr<Vector2> collisionNormal;

};

#endif

