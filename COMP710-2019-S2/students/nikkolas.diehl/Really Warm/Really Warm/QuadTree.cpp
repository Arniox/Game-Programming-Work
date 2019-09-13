#include "QuadTree.h"

using namespace std;

QuadTree::QuadTree()
{
}
QuadTree::QuadTree(Rectangle * boundry, int capacity)
	: mo_boundry(0)
	, mo_i_capacity(0)
	, mo_v_points()
	, mo_b_divided(false)
	, mo_i_arrayCounter(0)
	, northEast()
	, northWest()
	, southEast()
	, southWest()
{
	mo_boundry = boundry;
	mo_i_capacity = capacity;
	mo_b_divided = false;
}
QuadTree::~QuadTree()
{
	(*mo_v_points) = 0;

	northEast = 0;
	northWest = 0;
	southEast = 0;
	southWest = 0;

	delete mo_boundry;
	mo_boundry = 0;
}

void
QuadTree::Insert(Vector2* p) {
	//Check if point exists in boundry
	if (!mo_boundry->Contains(p)) {
		return;
	}

	//If points are under the limit then push
	if (mo_i_arrayCounter < mo_i_capacity) {
		mo_v_points[mo_i_arrayCounter] = p;
		mo_i_arrayCounter++;
	}
	else {
		//If not divided then divide
		if (!mo_b_divided) {
			SubDivide();
		}

		northEast->Insert(p);
		northWest->Insert(p);
		southEast->Insert(p);
		southWest->Insert(p);
	}
}

void
QuadTree::SubDivide() {
	double x = mo_boundry->X();
	double y = mo_boundry->Y();
	double w = mo_boundry->W();
	double h = mo_boundry->H();

	//Northwest boundry
	Rectangle* ne = new Rectangle(x + w / 2, y - h / 2, w / 2, h / 2);
	//Northeast boundry
	Rectangle* nw = new Rectangle(x - w / 2, y - h / 2, w / 2, h / 2);
	//Southwest boundry
	Rectangle* se = new Rectangle(x + w / 2, y + h / 2, w / 2, h / 2);
	//Southeast boundry
	Rectangle* sw = new Rectangle(x - w / 2, y + h / 2, w / 2, h / 2);

	northEast = new QuadTree(ne, mo_i_capacity);
	northWest = new QuadTree(nw, mo_i_capacity);
	southEast = new QuadTree(se, mo_i_capacity);
	southWest = new QuadTree(sw, mo_i_capacity);

	mo_b_divided = true;
}