#include "AI.h"
#include "Weapon.h"

using namespace std;
#define min(a, b) (((a) < (b)) ? (a) : (b))
#define max(a, b) (((a) > (b)) ? (a) : (b))

AI::AI()
: currentState(HOLDING_NOTHING)
, tryCounter(0)
, windowWidth(0)
, windowHeight(0)
{
}
AI::~AI()
{
}

void 
AI::CreateAI(IniParser* iniParser, BackBuffer* m_pBackBuffer, double x, double y, int screenWidth, int screenHeight)
{
	windowWidth = screenWidth;
	windowHeight = screenHeight;

	entityBox = new BoundingBox();
	double scale = iniParser->GetValueAsDouble("Player", "scale");
	std::string spriteSheet = iniParser->GetValueAsString("AI", "spriteSheet");
	movementForce = iniParser->GetValueAsDouble("AI", "forceLimit");

	Initialise(m_pBackBuffer->CreateAnimatedSprite(spriteSheet.c_str(),
		FRAME_COUNT, PANNEL_COUNT, FRAME_SPEED));

	SetSize(mx_animatedSprite->GetFrameWidth() * scale, mx_animatedSprite->GetFrameHeight() * scale);
	SetPos(x, y);

	//Set type
	mat->TYPE = mat->ENEMY;
	mat->movementSpeedLimit = iniParser->GetValueAsDouble("AI", "movementSpeedLimit");
	accuracyOffset = iniParser->GetValueAsDouble("AI", "accuracyOffset");

	//Set material attributes
	SetBaseStats(iniParser, scale);
}

void 
AI::ProcessAI(vector<Weapon*> weapons, Player* user, double deltaTime)
{
	//Set previous values
	playerPreviouslyAttached = alreadyAttached;

	if (!(mx_i_health <= 0)) {
		//Reset the movement variables
		ResetMovement();

		//Is AI holding anything
		(this->alreadyAttached ? currentState = HOLDING_GUN : currentState = HOLDING_NOTHING);

		//If the AI has completed an action, then set tryCount to 0
		if (playerPreviouslyAttached != alreadyAttached) {
			tryCounter = 0.0;
		}

		if (tryCounter < 5.0) {
			if (currentState == HOLDING_NOTHING) {
				Entity* closestWeapon = FindClosestWeapon(weapons);

				if (!(closestWeapon == nullptr)) {
					if (!closestWeapon->GetPos()->ValuesEqual(closestWeapon->GetPos()->x, this->GetPos()->x)) {
						//Move right
						if (closestWeapon->GetPos()->x > this->GetPos()->x) {
							MOVE_RIGHT = true;
						}
						//Move left
						if (closestWeapon->GetPos()->x < this->GetPos()->x) {
							MOVE_LEFT = true;
						}
					}
					
					tryCounter += deltaTime;
				}
				else {
					currentState = NOT_CARING;
				}

			}
			else if (currentState == HOLDING_GUN || currentState == NOT_CARING) {
				double distanceToUser = DistanceToUser(user);

				if (distanceToUser < 200) {
					aimingAngle = this->GetPos()->AngleDeg(static_cast<Player*>(user)->GetPos());
					aimingAngle += (static_cast<double>(rand()) / (RAND_MAX / accuracyOffset)) - (accuracyOffset / 2);

					FIRE_GUN = true;
				}
				else {
					if (!user->GetPos()->ValuesEqual(user->GetPos()->x, this->GetPos()->x)) {
						//Move right
						if (user->GetPos()->x > this->GetPos()->x) {
							MOVE_RIGHT = true;
						}
						//Move left
						if (user->GetPos()->y < this->GetPos()->x) {
							MOVE_LEFT = true;
						}
					}

					tryCounter += deltaTime;
				}

				//Throw weapon
				if (static_cast<Weapon*>(weaponAttached)->GetRemainingBullets() <= 0) {
					THROW_GUN = true;
					currentState = HOLDING_NOTHING;
				}
			}
		}
		else {
			//Preferentially move right to try and fix itself (this could be beter)
			MOVE_RIGHT = true;
			tryCounter += deltaTime;
			
			//Distance to wall
			double distanceToRightWall = windowWidth - this->GetPos()->x;
			distanceToRightWall /= windowWidth;
			double distanceToLeftWall = this->GetPos()->x;
			distanceToLeftWall /= windowWidth;

			if (tryCounter > min(distanceToRightWall, distanceToLeftWall) * 10.0) {
				tryCounter = 0.0;
			}
		}
		
		

		//Process
		Player::Process(deltaTime);
	}
	else {
		mat->dead = true;
		mat->doNotProcess = true;

		requestDeattachment = true;
	}
}

Entity*
AI::FindClosestWeapon(std::vector<Weapon*> weapons)
{
	double distance = INFINITY;
	Entity* weaponToReturn = nullptr;

	for (std::vector<Weapon*>::iterator it = weapons.begin(); it != weapons.end(); ++it) {
		if (this->GetPos()->Distance((*it)->GetPos()) < distance) {
			if (!(*it)->IsAttached()) {
				if (!((*it)->GetRemainingBullets() <= 0)) {
					weaponToReturn = (*it);
				}
			}
		}
	}

	//Return closest weapon
	return weaponToReturn;
}

double 
AI::DistanceToUser(Player* user)
{
	return this->GetPos()->Distance(user->GetPos());
}
