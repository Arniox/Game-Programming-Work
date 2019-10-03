#ifndef __QUADTREE_H__
#define __QUADTREE_H__

#include "Vector2.h"
#include "logmanager.h"
#include "Rectangle.h"
#include "entity.h"

//Libraries
#include <unordered_map>
#include <cmath>
#include <string>
#include <vector>

class QuadTree
{
protected:
	//Values stored for each quadtree
	int mo_i_capacity;
	int mo_i_depth = 4;
	std::unique_ptr<Rectangle> mo_boundry;

	int mo_i_arrayCounter;
	std::unordered_map<Entity*, Rectangle*> mo_v_AABBList;
	bool mo_b_divided;

	//Subdivided quadtrees
	QuadTree* northWest;
	QuadTree* northEast;
	QuadTree* southWest;
	QuadTree* southEast;


public:
	QuadTree();
	QuadTree(double x, double y, double w, double h, int capacity, int max_depth);
	~QuadTree();

	//Parse the entity list into the quadtree
	void ParseEntityList(std::vector<Entity*> entityList);
	//Insert function
	void Insert(Entity* entityToReference, Rectangle* AABB);
	//Remove and reinsert
	void RemoveReinsert();
	//Subdevide function
	void SubDivide();
	//Query an area
	void Query(Rectangle* range, std::vector<Entity*>& found, Entity* forThisEntity);
	//Clear quadtree for re-insertion
	void ClearTree();

	//Drawing for debugging
	void Draw(BackBuffer* backBuffer, int windowWidth, int windowHeight);

};

#endif
