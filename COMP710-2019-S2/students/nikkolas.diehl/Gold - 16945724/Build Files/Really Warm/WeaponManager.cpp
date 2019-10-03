#include "WeaponManager.h"

WeaponManager::WeaponManager()
: weaponList()
{
}
WeaponManager::~WeaponManager()
{
	for (std::vector<Weapon*>::iterator it = weaponList.begin(); it != weaponList.end(); ++it) {
		delete (*it);
		(*it) = nullptr;
	}

	weaponList.clear();
	weaponList.shrink_to_fit();
}

void 
WeaponManager::Initialise(IniParser * m_iniParser, BackBuffer * m_pBackBuffer, int screenWidth, int screenHeight)
{
	int randomWeaponCount = (rand() % 5) + 5; //Between 10 and 5 weapons
	for (int i = 0; i < randomWeaponCount; ++i) {
		//Spawn new weapons
		weaponList.push_back(new Weapon());

		//For now, spawn weapons at random places within the screen
		int xPos = (rand() % screenWidth - 26) + 13;
		int yPos = (rand() % screenHeight - 26) + 13;

		//Get random type
		Weapon::WeaponType randomType = static_cast<Weapon::WeaponType>(rand() % Weapon::COUNT);
		weaponList.back()->CreateWeapon(m_iniParser, m_pBackBuffer,
			xPos, yPos, 1.0, randomType);
	}
}

void 
WeaponManager::Process(InputHandler* m_inputHandler, double deltaTime)
{
	for (std::vector<Weapon*>::iterator it = weaponList.begin(); it != weaponList.end(); ++it) {
		if ((*it)->GetAttachement() != nullptr) {
			(*it)->Process(m_inputHandler, deltaTime,
				static_cast<Player*>((*it)->GetAttachement())->GetAimingAngle(),
				static_cast<Player*>((*it)->GetAttachement())->GetFireGun());
		}
		else {
			(*it)->Process(m_inputHandler, deltaTime, 0, false);
		}

		std::vector<Bullet*> weaponMag = (*it)->GetMagazine();
		for (std::vector<Bullet*>::iterator bu = weaponMag.begin(); bu != weaponMag.end(); ++bu) {
			(*bu)->IncrementAge();
		}
	}
}

void 
WeaponManager::Draw(BackBuffer & backBuffer)
{
	for (std::vector<Weapon*>::iterator it = weaponList.begin(); it != weaponList.end(); ++it) {
		(*it)->Draw(backBuffer);
	}
}

std::vector<Weapon*> 
WeaponManager::GetWeapons()
{
	return weaponList;
}
