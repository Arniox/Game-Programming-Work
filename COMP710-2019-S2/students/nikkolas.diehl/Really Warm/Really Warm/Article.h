#pragma once
#include "entity.h"

class Article:
	public Entity
{
public:
	Article();
	~Article();

	void SetCenter(float x, float y);
	float GetSize();

	//Movement
	float mo_f_BurstX;
	float mo_f_BurstY;

protected:
	//Physical Attributes
	float mo_f_Mass;
	float mo_f_Size;
	float mo_f_MaxMovementSpeed;

	int mo_i_currentState;
};

