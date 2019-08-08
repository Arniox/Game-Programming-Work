#include "EnemyPool.h"

EnemyPool::EnemyPool()
	:m_pEnemies(0)
	,enemyIterator()
{
}
EnemyPool::~EnemyPool()
{
	//Destroy and clean the enemy pool
	for (int i = 0; i < m_pEnemies.size(); i++) {
		delete m_pEnemies[i];
		m_pEnemies[i] = 0;
	}
}

void 
EnemyPool::Initialise(int rows, int columns, BackBuffer* m_pBackBuffer, int shiftX, int shiftY)
{
	//Initialise the enemy pool with a defined amount of enemies
	int entityCount = 0;

	for (int i = 0; i < rows; i++) {
		for (int k = 0; k < columns; k++) {
			m_pEnemies.push_back(new Enemy());
			m_pEnemies.at(entityCount)->Initialise(m_pBackBuffer->CreateSprite("assets/Sprites/alienenemy.png"));
			m_pEnemies.at(entityCount)->SetCenter(k * 100 + shiftX, i * 100 + shiftY);

			entityCount++;
		}
	}
}

void 
EnemyPool::Process(float deltaTime)
{
	//Process the enemy pool
	enemyIterator = m_pEnemies.begin();
	while (enemyIterator != m_pEnemies.end()) {
		if ((*enemyIterator)->dead) {
			delete (*enemyIterator);
			enemyIterator = m_pEnemies.erase(enemyIterator);
		}
		else {
			enemyIterator++;
		}
	}
}

void 
EnemyPool::Draw(BackBuffer& backBuffer)
{
	//Draw the enemy pool
	for (int i = 0; i < m_pEnemies.size(); i++) {
		m_pEnemies.at(i)->Draw(backBuffer);
	}
}

std::vector<Enemy*> 
EnemyPool::GetEnemies()
{
	return m_pEnemies;
}

Enemy*
EnemyPool::GetEnemyAt(int input)
{
	return m_pEnemies.at(input);
}
