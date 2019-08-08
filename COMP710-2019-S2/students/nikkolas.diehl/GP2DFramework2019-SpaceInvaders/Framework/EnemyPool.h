#pragma once
#include "Enemy.h"
#include <vector>
#include "backbuffer.h"

class EnemyPool
{
public:
	EnemyPool();
	~EnemyPool();

	//Enemy vector
	std::vector<Enemy*> m_pEnemies;
	std::vector<Enemy*>::iterator enemyIterator;

	void EnemyPool::Initialise(int rows, int columns, BackBuffer* backBuffer, int shiftX, int shiftY);
	
	void EnemyPool::Process(float deltaTime);
	void EnemyPool::Draw(BackBuffer& backBuffer);

	std::vector<Enemy*> EnemyPool::GetEnemies();
	Enemy* EnemyPool::GetEnemyAt(int input);
};

