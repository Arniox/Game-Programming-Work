#pragma once

#include "Vector2.h"
#include "logmanager.h"
#include "Rectangle.h"

//Libraries
#include <cmath>
#include <string>
#include <vector>

class QuadTree
{
public:
	Vector2 mp_Point;
	Rectangle mp_Rectangle;

protected:
	//Values stored for each quadtree
	int mo_i_capacity;
	Rectangle* mo_boundry;

	int mo_i_arrayCounter;
	Vector2* mo_v_points[4] = {};
	bool mo_b_divided;

	//Protected subdevide function
	void SubDivide();

	//Subdivided quadtrees
	QuadTree* northWest;
	QuadTree* northEast;
	QuadTree* southWest;
	QuadTree* southEast;


public:
	QuadTree();
	QuadTree(Rectangle* boundry, int capacity);
	~QuadTree();

	//Insert
	void Insert(Vector2* p);
};

