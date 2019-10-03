#define _CRTDBG_MAP_ALLOC
#include "Magazine.h"
#include "logmanager.h"

Magazine::Magazine()
: magazineOfBullets()
, fireRate(0)
, bulletSpeed(0)
, fire(false)
{
}
Magazine::Magazine(IniParser* iniParser, BackBuffer * backBuffer, int bulletCount, double _fireRate, double weaponDamagePtc)
{
	fireRate = _fireRate;
	bulletSpeed = iniParser->GetValueAsDouble("Bullet", "bulletSpeed");
	remainingBullets = bulletCount;

	for (int i = 0; i < bulletCount; ++i) {
		magazineOfBullets.push_back(new Bullet());
		//Each bullet is given a percentage of the total weapon damage. If 10 bullets in total does 100% damage,
		//Then each bullet does 10% damage.
		magazineOfBullets.back()->InstatiateBullet(iniParser, backBuffer, weaponDamagePtc / bulletCount, 0.0, 0.0);
	}
}
Magazine::~Magazine()
{
	for (int i = 0; i < static_cast<signed int>(magazineOfBullets.size()); ++i) {
		delete magazineOfBullets.at(i);
		magazineOfBullets.at(i) = nullptr;
	}

	magazineOfBullets.clear();
	magazineOfBullets.shrink_to_fit();
}

void 
Magazine::Process(Vector2 * bulletExitPoint, double heading, bool shoot, double deltaTime)
{
	if (shoot)
	{
		//Slow down fire rate
		if (count <= 0) {
			//Go through bullets array and fire the newest dead one
			for (int i = 0; i < static_cast<signed int>(magazineOfBullets.size()); ++i) {
				if (magazineOfBullets.at(i)->Spawn(heading, bulletExitPoint, bulletSpeed)) {
					//The gun has actually fired
					fire = true;
					remainingBullets--;
					break;
				}
			}
			count = fireRate;
		}
		else {
			DecrementCount();
		}
	}
	else {
		DecrementCount();
	}
}

void
Magazine::DecrementCount() {
	//fire is no longer true
	fire = false;

	if (!(count <= 0)) {
		count -= 0.1;
	}
}

void
Magazine::Draw(BackBuffer& backBuffer)
{
	//Only draw the alive bullets
	for (int i = 0; i < static_cast<signed int>(magazineOfBullets.size()); ++i) {
		if (!magazineOfBullets.at(i)->IsDead()) {
			magazineOfBullets.at(i)->Draw(backBuffer, 255);
		}
	}
}

std::vector<Bullet*> 
Magazine::GetBulletEntities()
{
	return magazineOfBullets;
}

int 
Magazine::GetRemainingBullets()
{
	return remainingBullets;
}

bool 
Magazine::GetFire()
{
	return fire;
}
