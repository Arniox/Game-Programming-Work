#define _CRTDBG_MAP_ALLOC
#include "Weapon.h"
#include "logmanager.h"

Weapon::Weapon()
: magazine()
, entityAttachment()
, attached(false)
, pointOfExit()
, pleaseDoNotInteract(false)
, pickUpDelay(0)
, randomShake(0)
, shootGun(false)
{
}
Weapon::~Weapon()
{
	delete pointOfExit;
	pointOfExit = nullptr;
	delete magazine;
	magazine = nullptr;
	delete linearVelocity;
	linearVelocity = nullptr;
	delete force;
	force = nullptr;
	delete entityBox;
	entityBox = nullptr;
	entityAttachment = nullptr;
	delete mat;
	mat = nullptr;

	delete mo_sprite;
	mo_sprite = nullptr;
}

void 
Weapon::CreateWeapon(IniParser* iniParser, BackBuffer * backBuffer, double x, double y, double scale, WeaponType type)
{
	//Create empty mouse pos
	pointOfExit = new Vector2();
	entityBox = new BoundingBox();

	std::string spriteSheet = iniParser->GetValueAsString("Weapon - " + std::to_string(type), "spriteSheet");
	Initialise(backBuffer->CreateSprite(spriteSheet.c_str()));

	SetSize(mo_sprite->GetWidth() * scale, mo_sprite->GetHeight() * scale);
	SetPos(x, y);

	//Set material attributes
	mat->dead = false;
	mat->constrained = iniParser->GetValueAsBoolean("Weapon - " + std::to_string(type), "constrained");
	mat->mass = iniParser->GetValueAsDouble("Weapon - " + std::to_string(type), "mass");
	mat->restitution = iniParser->GetValueAsDouble("Weapon - " + std::to_string(type), "restitution");
	mat->TYPE = mat->WEAPON;
	
	//Set up physics
	linearVelocity = new Vector2();
	entityAngle = 0;
	anglularVelocity = 0;
	force = new Vector2();
	CalculateBoxIntertia();

	//Set up magazine	
	magazine = new Magazine(iniParser, backBuffer,
		iniParser->GetValueAsInt("Weapon - " + std::to_string(type), "bulletCount"), 
		iniParser->GetValueAsDouble("Weapon - " + std::to_string(type), "fireRate"),
		iniParser->GetValueAsDouble("Weapon - " + std::to_string(type), "damage"));

	randomShake = iniParser->GetValueAsDouble("Weapon - " + std::to_string(type), "shake");
}

void 
Weapon::Process(InputHandler * inputHandler, double deltaTime, double aimingAngle, bool _shootGun)
{
	//Check if the weapon is attached at all
	if (attached) {
		//Check if the player has fired
		shootGun = _shootGun;

		//Set current attached item position to the item it's attached to and skip physics calculations
		SetPos(entityAttachment->GetPos()->x - ((entityBox->W() / 2) - entityAttachment->GetCollisionBox()->W() / 2),
			   entityAttachment->GetPos()->y - ((entityBox->H() / 2) - entityAttachment->GetCollisionBox()->H() / 2));

		//Set the gun angle to that of the mouse heading normalized
		entityAngle = aimingAngle;
		CalculateBulletExit();

		//Set slight random angle
		if (shootGun) {
			entityAngle += (static_cast<double>(rand()) / (RAND_MAX / randomShake)) - (randomShake / 2);
		}

		//Sprite rotation correction so it always looks like the gun sprite is always been held the right way
		if (entityAngle < -90 || entityAngle > 90) {
			mo_sprite->flipSprite = SDL_FLIP_VERTICAL;
		}
		else {
			mo_sprite->flipSprite = SDL_FLIP_NONE;
		}

		//If the attached entity requests a de-attach, then release
		if (static_cast<Player*>(entityAttachment)->requestDeattachment) {
			DettachFrom(entityAttachment);

			//Throw distance determined by the mass of the weapon
			linearVelocity->x = cos(degreesToRadians(entityAngle)) * (500.0 - mat->mass * 2);
			linearVelocity->y = sin(degreesToRadians(entityAngle)) * (500.0 - mat->mass * 2);

			pleaseDoNotInteract = true;
			pickUpDelay = 10;
		}
	}

	//If the gun does not want collision, then add a small pick up delay
	if (pleaseDoNotInteract) {
		if (pickUpDelay != 0) {
			pickUpDelay--;
		}
		else {
			pleaseDoNotInteract = false;
		}
	}

	//Process the magazine, and bullets
	if (attached) {
		magazine->Process(pointOfExit, entityAngle, shootGun, deltaTime);
	}
}

void
Weapon::Draw(BackBuffer& backBuffer)
{
	//Custom draw functions
	magazine->Draw(backBuffer);

	Entity::Draw(backBuffer, 255);
}

Weapon::WeaponType*
Weapon::GetWeaponType()
{
	return &weapon_type;
}

void
Weapon::CalculateBulletExit()
{
	double centerX = GetPos()->x + GetCollisionBox()->W() / 2;
	double centerY = GetPos()->y + GetCollisionBox()->H() / 2;
	double radius = GetCollisionBox()->W() / 2;

	double radiansDeg = degreesToRadians(entityAngle);

	pointOfExit->x = (centerX + radius * cos(radiansDeg));
	pointOfExit->y = (centerY + radius * sin(radiansDeg));
}

bool
Weapon::IsAttached()
{
	return attached;
}

void 
Weapon::AttachTo(Entity * entityToAttach)
{
	attached = true;

	//Add mass and attach
	entityToAttach->GetMat()->mass += this->GetMat()->mass;
	entityAttachment = entityToAttach;

	//Give player attachment
	static_cast<Player*>(entityAttachment)->AttachTo(this);

	//Player is now attached
	static_cast<Player*>(entityAttachment)->alreadyAttached = true;
}

void 
Weapon::DettachFrom(Entity * entityToAttach)
{
	attached = false;

	//Negate mass and de-attach
	static_cast<Player*>(entityToAttach)->alreadyAttached = false;
	entityToAttach->GetMat()->mass -= this->GetMat()->mass;

	//Player no longer holding weapon
	static_cast<Player*>(entityAttachment)->Deattach();

	entityAttachment = nullptr; //Clear the pointer
}

Entity* 
Weapon::GetAttachement()
{
	return entityAttachment;
}

std::vector<Bullet*> 
Weapon::GetMagazine()
{
	return magazine->GetBulletEntities();
}

int 
Weapon::GetRemainingBullets()
{
	return magazine->GetRemainingBullets();
}

bool 
Weapon::GetShoot()
{
	return shootGun;
}

bool 
Weapon::GetFireingTime()
{
	return magazine->GetFire();
}
