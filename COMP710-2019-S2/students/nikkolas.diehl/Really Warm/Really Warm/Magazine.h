#ifndef __MAGAZINE_H__
#define __MAGAZINE_H__

//Locals
#include "iniParser.h"
#include "backbuffer.h"
#include "entity.h"
#include "Bullet.h"

//Libraries


class Magazine
{
public:
	Magazine();
	Magazine(IniParser* iniParser, BackBuffer* backBuffer, int bulletCount, double _fireRate, double weaponDamagePtc);
	~Magazine();

	void Process(Vector2* bulletExitPoint, double heading, bool shoot, double deltaTime);
	void Draw(BackBuffer& backBuffer);
	std::vector<Bullet*> GetBulletEntities();

	int GetRemainingBullets();
	bool GetFire();

protected:
	std::vector<Bullet*> magazineOfBullets;
	double bulletSpeed = 0;
	double fireRate;
	double count = 0;

	int remainingBullets;
	
	bool fire;

	void DecrementCount();
};

#endif

