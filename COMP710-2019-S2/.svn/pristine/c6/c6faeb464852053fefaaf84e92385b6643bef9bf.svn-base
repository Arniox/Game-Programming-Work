#include "InGameScene.h"
#include <time.h>


InGameScene::InGameScene()
:score(0)
{

}
InGameScene::~InGameScene()
{
	//Delete the enemy pool
	delete m_enemyPool;
	m_enemyPool = 0;

	//Empty the bullet pool
	delete m_bulletPool;
	m_bulletPool = 0;

	//Empty the explosion pool
	delete m_explosionPool;
	m_explosionPool = 0;

	//Empty the particle manager
	delete m_paritcleManager;
	m_paritcleManager = 0;

	//Delete explosion coords
	m_explosionCoords.empty();

	//Empty sound
	UnloadFMOD();

	//Empty player
	delete m_pPlayerShip;
	m_pPlayerShip = 0;
}

void 
InGameScene::Initialise(IniParser* m_iniParser, BackBuffer* m_pBackBuffer, int& screenWidth, int& screenHeight)
{
	//Init Fmod
	InitFMOD();

	//--------------Member data---------------------
	m_windowWidth = screenWidth;
	m_windowHeight = screenHeight;

	//Set shift values for centering things
	int rows = 4;
	int columns = 14;
	float shiftX = (m_windowWidth / 2) - ((columns / 2) * 100);
	float shiftY = (m_windowHeight / 10);

	//Create the player ship instance.
	m_pPlayerShip = new PlayerShip;
	m_pPlayerShip->Initialise(m_pBackBuffer->CreateSprite("assets/Sprites/playership.png"));
	//Set play position
	m_pPlayerShip->SetPositionX(m_windowWidth / 2);
	m_pPlayerShip->SetPositionY(m_windowHeight - (m_windowHeight) / 10);

	//Init the enemy pool
	m_enemyPool = new EnemyPool();
	m_enemyPool->Initialise(rows, columns, m_pBackBuffer, shiftX, shiftY);
	//Init the bullet pool
	m_bulletPool = new BulletPool();
	m_bulletPool->Initialise(m_pBackBuffer);
	//Init the explosion pool
	m_explosionPool = new ExplosionPool();
	m_explosionPool->Initialise(m_pBackBuffer);
	//Init the particle manager
	m_paritcleManager = new ParticleManager();
	m_paritcleManager->Initialise(m_pBackBuffer, m_iniParser);
}

void
InGameScene::Process(float deltaTime)
{
	// Update the game world simulation:
	float bulletX = 0;
	float bulletY = 0;
	float enemyX = 0;
	float enemyY = 0;
	// Process death collision
	for (int i = 0; i < m_enemyPool->GetEnemies().size(); i++) {
		for (int k = 0; k < m_bulletPool->GetBullets().size(); k++) {
			//If the two objects are right next to eachother (each is roughly 15 pixels big)
			bulletX = m_bulletPool->GetBulletAt(k)->GetPositionX();
			bulletY = m_bulletPool->GetBulletAt(k)->GetPositionY();
			enemyX = m_enemyPool->GetEnemyAt(i)->GetPositionX();
			enemyY = m_enemyPool->GetEnemyAt(i)->GetPositionY();
			float distance = sqrt(pow(bulletX - enemyX, 2) + pow(bulletY - enemyY, 2)) - 15 - 5;
			if (distance < 3) {
				//Delete at index
				m_enemyPool->GetEnemyAt(i)->dead = true;
				m_bulletPool->GetBulletAt(k)->dead = true;
				explode = true;
				//Save the x and y coords of all the explosions
				m_explosionCoords.push_back(new std::vector<int>);
				m_explosionCoords.back()->push_back(enemyX);
				m_explosionCoords.back()->push_back(enemyY);

				score++;
			}
		}
	}

	//Process enemies
	m_enemyPool->Process(deltaTime);
	//Process bullets
	m_bulletPool->Process(m_fmodSystem, sFireSound, deltaTime, shoot, m_pPlayerShip->GetPositionX(), m_pPlayerShip->GetPositionY());
	//Process Explosions
	m_paritcleManager->Process(deltaTime, explode, m_explosionCoords, m_windowWidth, m_windowHeight);
	m_explosionPool->Process(m_fmodSystem, sExplosionSound, deltaTime, explode, m_explosionCoords);
	//Process Player
	m_pPlayerShip->Process(deltaTime, m_windowWidth, m_windowHeight, moveRight, moveLeft);
	//Process sound
	m_fmodSystem->update();
}

void 
InGameScene::Draw(BackBuffer& backBuffer, int& m_FPS, int& m_totalFramesPassed)
{
	backBuffer.SetClearColour(0, 0, 0);

	if (drawGame) {
		//Draw enemies in enemy pool
		m_enemyPool->Draw(backBuffer);
		//Draw all bullets in container...
		m_bulletPool->Draw(backBuffer);
		//Draw all explosions
		m_paritcleManager->Draw(backBuffer);
		m_explosionPool->Draw(backBuffer);

		//Draw the player ship...
		m_pPlayerShip->Draw(backBuffer);
	}
	else {
		//Win State
	}

	//Draw Text for title
	backBuffer.SetTextColour(255, 255, 255);
	string titleOutPut = "Score: " + to_string(score);
	backBuffer.DrawText(20, 10, titleOutPut.c_str(), 0);

	//Draw Text for fps
	backBuffer.SetTextColour(255, 255, 255);
	string fps = "Fps: " + to_string(m_FPS) + " | frameCount: ";
	fps += (m_totalFramesPassed > 1000000 ? " > 1M" : (m_totalFramesPassed > 100000 ? " > 100K" : (m_totalFramesPassed > 10000 ? " > 10k" : to_string(m_totalFramesPassed))));
	backBuffer.DrawText((m_windowWidth / 1.28), 10, fps.c_str(), 1);
}

void
InGameScene::InitFMOD()
{
	FMOD::System_Create(&m_fmodSystem);
	m_fmodSystem->init(32, FMOD_INIT_NORMAL, 0);

	m_fmodSystem->createSound("assets/Sounds/Shoot.wav", FMOD_DEFAULT, NULL, &sFireSound);
	m_fmodSystem->createSound("assets/Sounds/Explosion.wav", FMOD_DEFAULT, NULL, &sExplosionSound);
	m_fmodSystem->createSound("assets/Sounds/Move.wav", FMOD_DEFAULT, NULL, &sMoveSound);
}

void
InGameScene::UnloadFMOD()
{
	sFireSound->release();
	sExplosionSound->release();
	sMoveSound->release();
}

void
InGameScene::SetShoot(bool shoot)
{
	this->shoot = shoot;
}

void
InGameScene::SetMoveRight(bool right)
{
	this->moveRight = right;
}

void
InGameScene::SetMoveLeft(bool left)
{
	this->moveLeft = left;
}
