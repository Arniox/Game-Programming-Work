#define _CRTDBG_MAP_ALLOC
#include "Bullet.h"

Bullet::Bullet()
: bounceCount(0)
, dmgPct(0.0)
, age(0)
, previousDoNotProcess(false)
{
}
Bullet::~Bullet()
{
	delete entityBox;
	entityBox = nullptr;
	delete linearVelocity;
	linearVelocity = nullptr;
	delete force;
	force = nullptr;
	delete mat;
	mat = nullptr;

	delete mo_sprite;
	mo_sprite = nullptr;
}

void 
Bullet::InstatiateBullet(IniParser* iniParser, BackBuffer* backBuffer, double damagePercentage, double x, double y)
{
	entityBox = new BoundingBox();

	std::string sprite = iniParser->GetValueAsString("Bullet", "sprite");
	double scale = iniParser->GetValueAsDouble("Bullet", "scale");

	Initialise(backBuffer->CreateSprite(sprite.c_str()));

	SetSize(mo_sprite->GetWidth() * scale, mo_sprite->GetHeight() * scale);
	SetPos(x, y);

	//Set material attributes
	mat->dead = true;
	previousDoNotProcess = mat->doNotProcess;
	mat->constrained = iniParser->GetValueAsBoolean("Bullet", "constrained");
	mat->isPhase = true;
	mat->restitution = iniParser->GetValueAsDouble("Bullet", "restitution");
	mat->mass = iniParser->GetValueAsDouble("Bullet", "mass");
	mat->TYPE = mat->BULLET;

	//Set up physics
	linearVelocity = new Vector2();
	entityAngle = 0;
	anglularVelocity = 0;
	force = new Vector2();
	CalculateBoxIntertia();

	//Set up sprite data
	mo_sprite->SetSpriteScaler(scale);

	//Set up damage per bullet
	dmgPct = damagePercentage;
}

void
Bullet::DecrementBounce(int MAX_TIMESTEP)
{
	if (!(bounceCount >= MAX_TIMESTEP * bounceLimit)) {
		bounceCount++;
		//dmgPct = 0; //As soon as a bullet bounces at all, then remove all damage as it is now "dead"
	}
	else {
		//Keep alive but stop processing so you can get a build
		//up of bullets on the floor
		mat->doNotProcess = true;
	}
}

void 
Bullet::IncrementAge()
{
	if (!(mat->dead || mat->doNotProcess)) {
		age++;
	}
}

//When a bullet has been used or has done it's job, it will remove it's damage
void 
Bullet::DeleteDamage()
{
	dmgPct = 0;
}

bool 
Bullet::Spawn(double heading, Vector2* spawnPoint, double bulletSpeed)
{
	if (mat->dead) {

		//Make the bullet alive and then return true that the bullet has been spawned
		mat->dead = false;

		//Set position and initial velocity
		double xGoPos = spawnPoint->x - GetCollisionBox()->W() / 2;
		double yGoPos = spawnPoint->y - GetCollisionBox()->H() / 2;

		entityBox->X(xGoPos);
		entityBox->Y(yGoPos);

		linearVelocity->x = cos(degreesToRadians(heading)) * bulletSpeed;
		linearVelocity->y = sin(degreesToRadians(heading)) * bulletSpeed;

		entityAngle = 180 + heading;

		return true;
	}
	else {
		return false;
	}
}

bool 
Bullet::DidBounce()
{
	if (previousDoNotProcess != mat->doNotProcess) {
		previousDoNotProcess = mat->doNotProcess;
		return true;
	}
	else {
		return false;
	}
}

double
Bullet::GetDamagePct()
{
	if (age > MIN_AGE) {
		return dmgPct;
	}
	else {
		return 0;
	}
}
