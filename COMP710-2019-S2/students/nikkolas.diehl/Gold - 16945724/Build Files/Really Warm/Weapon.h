#ifndef __WEAPON_H__
#define __WEAPON_H__

//Locals
#include "iniParser.h"
#include "backbuffer.h"
#include "entity.h"
#include "inputhandler.h"
#include "animatedsprite.h"
#include "Magazine.h"
#include "Player.h"

//Libraries
#include <string>
#include <cassert>

class Weapon:
	public Entity
{
public:
	Weapon();
	~Weapon();

	//Weapon type with drop chance
	enum WeaponType {
		SHOTGUN,
		PISTOL,
		SMG,
		MINIGUN,

		COUNT
	};
	WeaponType weapon_type;

	void CreateWeapon(IniParser* iniParser, BackBuffer* m_pBackBuffer, double x, double y, double scale, WeaponType type);
	void Process(InputHandler* inputHandler, double deltaTime, double aimingAngle, bool _shootGun);
	void Draw(BackBuffer& m_pBackBuffer);

	//Get weapon type
	WeaponType* GetWeaponType();

	//Attach weapon to an entity
	bool IsAttached();
	void AttachTo(Entity* entityToAttach);
	void DettachFrom(Entity * entityToAttach);
	Entity* GetAttachement();

	//Do not collide or interact with the player
	bool pleaseDoNotInteract;
	int pickUpDelay;

	//Get magazine
	std::vector<Bullet*> GetMagazine();
	//Get bullets left
	int GetRemainingBullets();
	//Get shoot
	bool GetShoot();
	bool GetFireingTime();

private:
	Entity* entityAttachment;
	bool attached;

	double randomShake;
	bool shootGun;

	//Bullet exit pos
	Vector2* pointOfExit;
	void CalculateBulletExit();

protected:
	Magazine* magazine;
};

#endif

