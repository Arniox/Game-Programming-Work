#include "BulletPool.h"

BulletPool::BulletPool()
	:m_pBullets(0)
	,bulletIterator()
{
}
BulletPool::~BulletPool()
{
	//Destroy and clean the bullet pool
	for (int i = 0; i < m_pBullets.size(); i++) {
		delete m_pBullets[i];
		m_pBullets[i] = 0;
	}
}

void 
BulletPool::Initialise(BackBuffer* m_pBackBuffer)
{
	//Initialise the object with a set maximum amount of bullets
	for (int i = 0; i < MAX_BULLETS; i++) {
		m_pBullets.push_back(new Bullet());
		m_pBullets.at(i)->Initialise(m_pBackBuffer->CreateSprite("assets/Sprites/playerbullet.png"));
	}
}

void 
BulletPool::Process(FMOD::System* m_fmodSystem, FMOD::Sound* sFireSound, float deltaTime, bool& shoot, float posX, float posY)
{
	//Shoot only dead bullets
	if (shoot) {
		Bullet* bullet = GetNewestDead();
		if (bullet != 0) {
			//In use now
			bullet->dead = false;
			bullet->SetCenter(posX, posY);
			//Play fire
			m_fmodSystem->playSound(sFireSound, 0, false, 0);

			shoot = false;
		}
		//Else no other bullets can be used
	}

	//Process each bullet if they're alive
	float bulletVelocity = 150;

	for (int k = 0; k < m_pBullets.size(); k++) {
		float currentY = m_pBullets.at(k)->GetPositionY();

		//If alive process
		if (!m_pBullets.at(k)->dead) {
			m_pBullets.at(k)->SetPositionY(currentY - bulletVelocity * deltaTime);
		}

		//If hit top then kill
		if (m_pBullets.at(k)->GetPositionY() < 15) {
			m_pBullets.at(k)->dead = true;
		}
	}
}

void 
BulletPool::Draw(BackBuffer& backBuffer)
{
	//Draw each bullet if they alive
	for (int i = 0; i < m_pBullets.size(); i++) {
		if (!m_pBullets.at(i)->dead) {
			m_pBullets.at(i)->Draw(backBuffer);
		}
	}
}

std::vector<Bullet*> 
BulletPool::GetBullets()
{
	return m_pBullets;
}

Bullet*
BulletPool::GetBulletAt(int input)
{
	return m_pBullets.at(input);
}

Bullet* 
BulletPool::GetNewestDead()
{
	//Return the last dead (unused) bullet
	for (int i = 0; i < m_pBullets.size(); i++) {
		if (m_pBullets.at(i)->dead) {
			return m_pBullets.at(i);
		}
	}

	//If found nothing, then return null
	return 0;
}
