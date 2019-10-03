#ifndef __WEAPONMANAGER_H__
#define __WEAPONMANAGER_H__

//Locals
#include "iniParser.h"
#include "entity.h"
#include "Weapon.h"
#include "Bullet.h"

//Libraries
#include <vector>

class WeaponManager
{
public:
	WeaponManager();
	~WeaponManager();

	void Initialise(IniParser* m_iniParser, BackBuffer* m_pBackBuffer, int screenWidth, int screenHeight);
	void Process(InputHandler* m_inputHandler, double deltaTime);
	void Draw(BackBuffer& backBuffer);

	std::vector<Weapon*> GetWeapons();

private:
	std::vector<Weapon*> weaponList;
};

#endif

