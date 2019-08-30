#pragma once
#include "entity.h"

class Article:
	public Entity
{
public:
	Article();
	~Article();

	void SetCenter(float x, float y);
	Sprite* GetSprite();
	float GetSize();

	//Movement
	float mp_f_BurstX;
	float mp_f_BurstY;

	//Article sprite height and width
	int mp_i_SpriteHeight;
	int mp_i_SpriteWidth;

protected:
	//Physical Attributes
	float mo_f_Mass;
	float mo_f_Size;
	float mo_f_MaxMovementSpeed;

	int mo_i_currentState;
};

