#define _CRTDBG_MAP_ALLOC
#include "InGameScene.h"
#include "AIManager.h"

InGameScene::InGameScene()
: mx_player(0)
, weaponManager()
, entityProcessingUnit()
, quadTree()
, screenBoundry()
, totalEntityCount(0)
, processedEntities(0)
, m_processesPerFrame(0)
, drawQuadTree(false)
, pauseInGameScene(false)
, firstTimeToPlay(true)
, youLooseScene(false)
, infoBar()
, aiManager()
{
}
InGameScene::~InGameScene()
{
	delete aiManager;
	aiManager = nullptr;

	delete infoBar;
	infoBar = nullptr;

	delete m_wallController;
	m_wallController = nullptr;

	delete entityProcessingUnit;
	entityProcessingUnit = nullptr;

	delete mx_player;
	mx_player = nullptr;

	delete weaponManager;
	weaponManager = nullptr;

	sceneEntities.clear();
	sceneEntities.shrink_to_fit();

	delete quadTree;
	quadTree = nullptr;
	delete screenBoundry;
	screenBoundry = nullptr;

	//Delete all sounds and clear
	ReleaseSounds();
}

void 
InGameScene::Initialise(FMOD::System* m_fModSystem, IniParser* m_iniParser, BackBuffer* m_pBackBuffer, int screenWidth, int screenHeight)
{
	screenBoundry = new Rectangle(0, 0, static_cast<double>(screenWidth), static_cast<double>(screenHeight));
	infoBar = new UIElement();
	infoBar->CreateUIElement(m_iniParser->GetValueAsString("IngameUI", "infoBarSprite"), m_pBackBuffer, " ", 
		static_cast<int>(screenWidth * 0.50), 0,
		1, 2);

	//--------------Member data---------------------
	m_windowWidth = screenWidth;
	m_windowHeight = screenHeight;

	//--------------Ingame entities-----------------
	mx_player = new User();
 	mx_player->CreateUser(m_iniParser, m_pBackBuffer, (screenBoundry->W() / 2.0) - 10.0, 100.0);

	//--------------Walls---------------------------
	m_wallController = new WallController();
	m_wallController->Initialise(m_iniParser, m_pBackBuffer, screenBoundry->W(), screenBoundry->H());

	//--------------AI------------------------------
	aiManager = new AIManager();
	aiManager->Initialise(m_iniParser, m_pBackBuffer, screenWidth, screenHeight);

	//--------------Weapons-------------------------
	weaponManager = new WeaponManager();
	weaponManager->Initialise(m_iniParser, m_pBackBuffer, screenWidth, screenHeight);

	//Insert all objects into vector (TEMP. later a quadtree)
	InsertAll();

	//Create all sounds
	CreateSounds(m_fModSystem, m_iniParser);

	//Processing unit
	entityProcessingUnit = new EntityProcessingUnit();
	entityProcessingUnit->Initialise(&sceneEntities);

	//--------------Quad Tree-----------------------
	drawQuadTree = m_iniParser->GetValueAsBoolean("Debugging", "drawQuadTree");
	quadTree = new QuadTree(screenBoundry->X(),	 //X
							screenBoundry->Y(),	 //Y
							screenBoundry->W(),	 //W
							screenBoundry->H(),	 //H
							6, 5);				 //Capacity and max depth
	
}

void
InGameScene::Process(FMOD::System* m_fModSystem, FMOD::ChannelGroup* effectsChannelGroup, FMOD::Channel* effectsChannel, InputHandler* ep_inputHander, float deltaTime, int screenWidth, int screenHeight)
{
	//Set up values and bools for sound and effects
	processedEntities = 0;
	playerPreviousJumped = mx_player->DidJump();
	previouslyInAir = mx_player->GetMat()->inAir;

	//Clear quadtree
	quadTree->ClearTree();
	//Re-insert everything
	quadTree->ParseEntityList(sceneEntities);

	//Process entities
	//Player
	mx_player->ProcessUser(ep_inputHander, deltaTime);

	//Entity processing unit
	entityProcessingUnit->Process(quadTree, deltaTime, screenWidth, screenHeight);

	//Weapons
	weaponManager->Process(ep_inputHander, deltaTime);

	//Proccess AI
	aiManager->Process(weaponManager->GetWeapons(), mx_player, deltaTime);

	//Count processed entities
	for (std::vector<Entity*>::iterator it = sceneEntities.begin(); it != sceneEntities.end(); ++it) {
		if (!((*it)->IsDead() || (*it)->GetMat()->doNotProcess || !(*it)->GetMat()->constrained)) {
			processedEntities++;
		}
	}

	//Update ingame UI
	std::string HUD = "";
	if (mx_player->GetAttachement() == nullptr) {
		HUD += "H: " + std::to_string(mx_player->GetHealth()) + " A: 0";
	}
	else {
		int ammoCountOut = static_cast<Weapon*>(mx_player->GetAttachement())->GetRemainingBullets();

		HUD += "H: " + std::to_string(mx_player->GetHealth()) + " A: " + std::to_string((ammoCountOut < 0 ? 0 : ammoCountOut));
	}
	//Update Hud
	infoBar->UpdateElementText(HUD);
	
	//Play sounds
	PlaySounds(m_fModSystem, effectsChannelGroup, effectsChannel);

	//Request a pause from keyboard
	pauseInGameScene = ep_inputHander->GetKeyBoardLayout("escape");
}

void
InGameScene::Draw(BackBuffer& backBuffer, int& m_FPS, int& m_totalFramesPassed)
{
	//Update processes per frame every half frame
	if (m_totalFramesPassed % 30 == 0) {
		m_processesPerFrame = GetProcessingCount();
	}

	string HUDOutPut = "FPS: " + to_string(m_FPS) + " | PPF: " + to_string(m_processesPerFrame);
	string HUDOutPut2 = "E+: " + to_string(processedEntities) + "/" + to_string(totalEntityCount);

	backBuffer.DrawText(50, 50, HUDOutPut.c_str(), 2, false, 1000);
	backBuffer.DrawText(50, 80, HUDOutPut2.c_str(), 2, false, 1000);

	backBuffer.SetClearColour(85, 85, 85);

	//Draw entities
	//Player
	mx_player->Draw(backBuffer);
	//Walls
	m_wallController->Draw(backBuffer);
	//Weapons
	weaponManager->Draw(backBuffer);
	//AI
	aiManager->Draw(backBuffer);

	//Draw info bar
	infoBar->DrawUIElement(backBuffer, 255, false);

	//Quadtree
	if (drawQuadTree) {
		quadTree->Draw(&backBuffer, m_windowWidth, m_windowHeight);
	}

}

void
InGameScene::InsertAll()
{
	//clear vector first
	sceneEntities.clear();

	//Player
	sceneEntities.push_back(mx_player);
	//Walls
	for (int i = 0; i < static_cast<signed int>(m_wallController->GetWalls()->size()); ++i) {
		sceneEntities.push_back(m_wallController->GetWalls()->at(i));
	}
	//Weapons
	std::vector<Weapon*> weapons = weaponManager->GetWeapons();
	for (std::vector<Weapon*>::iterator it = weapons.begin(); it != weapons.end(); ++it) {
		sceneEntities.push_back((*it));

		//Bullets
		for (int i = 0; i < static_cast<signed int>((*it)->GetMagazine().size()); ++i) {
			sceneEntities.push_back((*it)->GetMagazine().at(i));
		}
	}
	//AI
	std::vector<AI*> ai = aiManager->GetAI();
	for (std::vector<AI*>::iterator it = ai.begin(); it != ai.end(); ++it) {
		sceneEntities.push_back((*it));
	}

	totalEntityCount = sceneEntities.size();
}

void 
InGameScene::PlaySounds(FMOD::System* m_fModSystem, FMOD::ChannelGroup* gameChannelGroup, FMOD::Channel* effectsChannel)
{
	//Play reload noise when you pick up a weapon
	if (mx_player->GetPreviousAttachment() != mx_player->alreadyAttached && mx_player->GetPreviousAttachment() == false) {
		m_fModSystem->playSound(reload, gameChannelGroup, false, &effectsChannel);
	}

	//For each enemy
	std::vector<AI*> enemyVec = aiManager->GetAI();
	for (std::vector<AI*>::iterator it = enemyVec.begin(); it != enemyVec.end(); ++it) {
		if ((*it)->GetPreviousAttachment() != (*it)->alreadyAttached && (*it)->GetPreviousAttachment() == false) {
			m_fModSystem->playSound(reload, gameChannelGroup, false, &effectsChannel);
		}
	}

	//Play gun sounds
	std::vector<Weapon*> weapons = weaponManager->GetWeapons();
	for (std::vector<Weapon*>::iterator it = weapons.begin(); it != weapons.end(); ++it) {
		//If the player/entity is holding a weapon, and the entity the weapon is attached to is the player/entity,
		//And if the weapon is being shot, and if the magazine delay count (based on fire rare) is less than 0,
		//Then play gun shot sound
		bool test = (*it)->GetShoot();

		//Play gun shot for player
		if ((*it)->GetShoot() && (*it)->GetFireingTime() && (*it)->IsAttached()) {
			m_fModSystem->playSound(gunShot, gameChannelGroup, false, &effectsChannel);
		}

		//Play shell falling sound when bullet is no longer processing
		std::vector<Bullet*> wepaonMag = (*it)->GetMagazine();
		for (std::vector<Bullet*>::iterator bu = wepaonMag.begin(); bu != wepaonMag.end(); ++bu) {
			if ((*bu)->DidBounce()) {
				m_fModSystem->playSound(shellsFalling, gameChannelGroup, false, &effectsChannel);
			}
		}
	}

	//Play jump grunt sound
	if (playerPreviousJumped != mx_player->DidJump() && playerPreviousJumped == false && !previouslyInAir) {
		m_fModSystem->playSound(grunt, gameChannelGroup, false, &effectsChannel);
	}
}

void
InGameScene::PlayInGameMusic(FMOD::System& m_fModSystem, FMOD::ChannelGroup* gameChannelGroup, FMOD::Channel* musicChannel)
{
	if (firstTimeToPlay) {
		firstTimeToPlay = false;
		m_fModSystem.playSound(ingameMusic, gameChannelGroup, false, &musicChannel);
	}
}

int 
InGameScene::GetProcessingCount()
{
	return entityProcessingUnit->GetProcessingCount();;
}

int 
InGameScene::GetTotalEntities()
{
	return totalEntityCount;
}

int 
InGameScene::GetEntitiesProcessed()
{
	return processedEntities;
}

void 
InGameScene::UnPause()
{
	pauseInGameScene = false;
}

bool
InGameScene::RequestPause()
{
	return pauseInGameScene;
}

bool 
InGameScene::RequestFailure()
{
	//Request a pause as win or loss
	if (mx_player->GetHealth() <= 0) {
		return true;
	}
	else {
		return false;
	}
}

bool 
InGameScene::RequestWin()
{
	return false;
}

void
InGameScene::CreateSounds(FMOD::System* m_fModSystem, IniParser* m_iniParser)
{
	//Sounds
	std::string ingameS = m_iniParser->GetValueAsString("IngameSounds", "ingameMusic");
	std::string miniGunS = m_iniParser->GetValueAsString("IngameSounds", "minigunSound");
	std::string gunShotS = m_iniParser->GetValueAsString("IngameSounds", "normalFireSound");
	std::string shellsFallingS = m_iniParser->GetValueAsString("IngameSounds", "shellsFallingSound");
	std::string shotGunS = m_iniParser->GetValueAsString("IngameSounds", "shotGunSound");
	std::string walkingS = m_iniParser->GetValueAsString("IngameSounds", "walkingSound");
	std::string reloadS = m_iniParser->GetValueAsString("IngameSounds", "reloadSound");
	std::string gruntS = m_iniParser->GetValueAsString("IngameSounds", "grunt");

	std::vector<std::string> randomDeaths;
	for (int i = 0; i < NUMBER_OF_DEATH_SOUNDS; ++i) {

		std::string soundName = "randomDeadSound" + std::to_string(i);
		randomDeaths.push_back(m_iniParser->GetValueAsString("IngameSounds", soundName));
	}

	//Bitwise operations for creating sounds
	//Non-looping sounds
	FMOD_MODE nonLooping = FMOD_LOOP_OFF | FMOD_2D | FMOD_3D_WORLDRELATIVE | FMOD_3D_INVERSEROLLOFF;
	FMOD_MODE looping = FMOD_LOOP_NORMAL | FMOD_2D | FMOD_3D_WORLDRELATIVE | FMOD_3D_INVERSEROLLOFF;

	//Create the sounds with the sound system
	//Ingame music that repeats
	m_fModSystem->createSound(ingameS.c_str(), looping, NULL, &ingameMusic);
	ingameMusic->setLoopCount(-1); //Infinite

	//Other sounds
	m_fModSystem->createSound(miniGunS.c_str(), nonLooping, NULL, &mingun);
	m_fModSystem->createSound(gunShotS.c_str(), nonLooping, NULL, &gunShot);
	m_fModSystem->createSound(shellsFallingS.c_str(), nonLooping, NULL, &shellsFalling);
	m_fModSystem->createSound(shotGunS.c_str(), nonLooping, NULL, &shotGun);
	m_fModSystem->createSound(walkingS.c_str(), nonLooping, NULL, &walking);
	m_fModSystem->createSound(reloadS.c_str(), nonLooping, NULL, &reload);
	m_fModSystem->createSound(gruntS.c_str(), nonLooping, NULL, &grunt);

	for (int i = 0; i < NUMBER_OF_DEATH_SOUNDS; ++i) {
		FMOD::Sound* go = randomDead[i];
		m_fModSystem->createSound(randomDeaths.at(i).c_str(), nonLooping, NULL, &go);
	}

	randomDeaths.clear();
	randomDeaths.shrink_to_fit();
}

void
InGameScene::ReleaseSounds()
{
	ingameMusic->release();
	mingun->release();
	gunShot->release();
	shotGun->release();
	shellsFalling->release();
	walking->release();
	reload->release();

	for (int i = 0; i < NUMBER_OF_DEATH_SOUNDS; ++i) {
		randomDead[i]->release();
	}
}