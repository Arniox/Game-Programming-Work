#define _CRTDBG_MAP_ALLOC
#include "QuadTree.h"

using namespace std;

QuadTree::QuadTree()
{
}
QuadTree::QuadTree(double x, double y, double w, double h, int capacity, int max_depth)
	: mo_boundry()
	, mo_i_capacity(0)
	, mo_i_depth(0)
	, mo_v_AABBList()
	, mo_b_divided(false)
	, mo_i_arrayCounter(0)
	, northEast()
	, northWest()
	, southEast()
	, southWest()
{
	mo_boundry = std::make_unique<Rectangle>();

	mo_boundry->Set(x, y, w, h);
	mo_i_capacity = capacity;
	mo_i_depth = max_depth;
	mo_b_divided = false;
}
QuadTree::~QuadTree()
{
	mo_boundry = nullptr;

	this->ClearTree();
}

void QuadTree::ParseEntityList(std::vector<Entity*> entityList)
{
	for (int i = 0; i < static_cast<signed int>(entityList.size()); ++i) {
		Insert(entityList.at(i), entityList.at(i)->GetCollisionBox());
	}
}

void
QuadTree::Insert(Entity* entityToReference, Rectangle* entityBox) {
	
	//Check if point exists in boundry
	if (!mo_boundry->Overlaps(entityBox) || entityToReference->GetMat()->dead || entityToReference->GetMat()->doNotProcess) {
		return;
	}

	//If the max depth hasn't been reached yet
	if (mo_i_depth >= 0) {
		//If points are under the limit then push
		if (mo_i_arrayCounter < mo_i_capacity) {
			mo_v_AABBList[entityToReference] = entityBox; //Add to map
			mo_i_arrayCounter++;
		}
		else {
			//If not divided then divide
			if (!mo_b_divided) {
				SubDivide();
				RemoveReinsert();
			}

			northEast->Insert(entityToReference, entityBox);
			northWest->Insert(entityToReference, entityBox);
			southEast->Insert(entityToReference, entityBox);
			southWest->Insert(entityToReference, entityBox);
		}
	}
	else {
		//Otherwise just insert the points anyways
		mo_v_AABBList[entityToReference] = entityBox; //Add to map
	}
	
}

void 
QuadTree::RemoveReinsert()
{
	for (std::unordered_map<Entity*, Rectangle*>::iterator it = mo_v_AABBList.begin(); it != mo_v_AABBList.end(); ++it) {
		//When subdividing, insert point into children
		northEast->Insert(it->first, it->second);
		northWest->Insert(it->first, it->second);
		southEast->Insert(it->first, it->second);
		southWest->Insert(it->first, it->second);

		//DO NOT set capacity back to 0.
		//Trick parent into giving items to children
	}

	//Clear the parent mo_v_AABBList array.
	mo_v_AABBList.clear();
}

void
QuadTree::SubDivide() {
	double x = mo_boundry->X();
	double y = mo_boundry->Y();
	double w = mo_boundry->W() / 2;
	double h = mo_boundry->H() / 2;

	//Northwest boundry (x + w, y    , w, h)
	//Northeast boundry (x    , y    , w, h)
	//Southwest boundry (x + w, y + h, w, h)
	//Southeast boundry (x    , y + h, w, h)

	northEast = new QuadTree(x + w, y    , w, h, mo_i_capacity, mo_i_depth - 1);
	northWest = new QuadTree(x    , y    , w, h, mo_i_capacity, mo_i_depth - 1);
	southEast = new QuadTree(x + w, y + h, w, h, mo_i_capacity, mo_i_depth - 1);
	southWest = new QuadTree(x    , y + h, w, h, mo_i_capacity, mo_i_depth - 1);

	mo_b_divided = true;
}

void
QuadTree::Query(Rectangle* range, std::vector<Entity*>& found, Entity* forThisEntity)
{
	//The current range doesn't overlap this boundry
	if (!range->Overlaps(&(*mo_boundry))) {
		return;
	}

	//For each point in this current quad
	for (std::unordered_map<Entity*, Rectangle*>::iterator it = mo_v_AABBList.begin(); it != mo_v_AABBList.end(); ++it) {
		//Make sure not to push the same entity that it's searching for
		if (it->first != forThisEntity) {
			//If it overlaps
			if (range->Overlaps(it->second)) {
				found.push_back(it->first);
			}
		}
	}

	if (mo_b_divided) {
		this->northEast->Query(range, found, forThisEntity);
		this->northWest->Query(range, found, forThisEntity);
		this->southEast->Query(range, found, forThisEntity);
		this->southWest->Query(range, found, forThisEntity);
	}
}

void 
QuadTree::ClearTree()
{
	mo_v_AABBList.clear();

	if (mo_b_divided) {
		this->northEast->ClearTree();
		delete northEast;
		northEast = 0;

		this->northWest->ClearTree();
		delete northWest;
		northWest = 0;

		this->southEast->ClearTree();
		delete southEast;
		southEast = 0;

		this->southWest->ClearTree();
		delete southWest;
		southWest = 0;
	}

	mo_i_arrayCounter = 0;
	mo_b_divided = false;
}

void 
QuadTree::Draw(BackBuffer* backBuffer, int windowWidth, int windowHeight)
{
	int wW = 0;
	int wH = 0;

	int x = static_cast<int>(mo_boundry->X()) - wW;
	int y = static_cast<int>(mo_boundry->Y()) - wH;
	int w = static_cast<int>(mo_boundry->W());
	int h = static_cast<int>(mo_boundry->H());

	backBuffer->SetDrawColour(255, 255, 0);

	backBuffer->DrawLine(x, y, x + w, y);
	backBuffer->DrawLine(x, y, x, y + h);
	backBuffer->DrawLine(x, y + h, x + w, y + h);
	backBuffer->DrawLine(x + w, y, x + w, y + h);

	if (mo_b_divided) {
		northEast->Draw(backBuffer, wW, wH);
		northWest->Draw(backBuffer, wW, wH);
		southEast->Draw(backBuffer, wW, wH);
		southWest->Draw(backBuffer, wW, wH);
	}
}
