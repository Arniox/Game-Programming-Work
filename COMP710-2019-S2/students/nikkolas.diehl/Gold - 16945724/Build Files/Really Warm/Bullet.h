#ifndef __BULLET_H__
#define __BULLET_H__

//Locals
#include "iniParser.h"
#include "backbuffer.h"
#include "entity.h"

class Bullet:
	public Entity
{
public:
	Bullet();
	~Bullet();

	void InstatiateBullet(IniParser* iniParser, BackBuffer* m_pBackbuffer, double damagePercentage, double x, double y);

	//Functions
	void DecrementBounce(int MAX_TIMESTEP);
	void IncrementAge();
	void DeleteDamage();
	
	//Getters
	bool Spawn(double heading, Vector2* spawnPoint, double bulletSpeed);
	bool DidBounce();
	double GetDamagePct();

private:
	int bounceCount;
	const int bounceLimit = 5;
	int age;
	const int MIN_AGE = 5;
	double dmgPct;

	bool previousDoNotProcess;
};

#endif

