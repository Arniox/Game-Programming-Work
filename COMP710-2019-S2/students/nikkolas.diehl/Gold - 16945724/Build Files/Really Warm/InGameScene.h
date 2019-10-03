#pragma once
#ifndef __INGAMESCENE_H__
#define __INGAMESCENE_H__

//Locals
#include "UIElement.h"
#include "fmod.hpp"
#include "backbuffer.h"
#include "iniParser.h"
#include "entity.h"
#include "Walls.h"
#include "WallController.h"
#include "inputhandler.h"
#include "QuadTree.h"
#include "Rectangle.h"
#include "inputhandler.h"
#include "EntityProcessingUnit.h"
#include "WeaponManager.h"
#include "User.h"
#include "AIManager.h"

//Libraries
#include <time.h>
#include <vector>

class InGameScene
{
public:
	InGameScene();
	~InGameScene();

	//Main functionality
	void Initialise(FMOD::System* m_fModSystem, IniParser* m_iniParser, BackBuffer* m_pBackBuffer, int screenWidth, int screenHeight);
	void Draw(BackBuffer& backBuffer, int& m_FPS, int& m_totalFramesPassed);
	void Process(FMOD::System* m_fModSystem, FMOD::ChannelGroup* effectsChannelGroup, FMOD::Channel* effectsChannel, InputHandler* ep_inputHander, float deltaTime, int screenWidth, int screenHeight);

	int GetProcessingCount();
	int GetTotalEntities();
	int GetEntitiesProcessed();

	void UnPause();
	bool RequestPause();
	bool RequestFailure();
	bool RequestWin();

	void PlayInGameMusic(FMOD::System& m_fModSystem, FMOD::ChannelGroup* musicChannelGroup, FMOD::Channel* musicChannel);

	//All entities exist in one entity vector
	//WIP (temporary. This will use quadtree later
	std::vector<Entity*>::iterator entitIter;
	std::vector<Entity*> sceneEntities;

private:
	//Ingame members
	WallController* m_wallController;
	User* mx_player;
	AIManager* aiManager;
	WeaponManager* weaponManager;

	//Private functions
	void CreateSounds(FMOD::System* m_fModSystem, IniParser* m_iniParser);
	void ReleaseSounds();
	void InsertAll();

protected:
	//Quad Tree functionality
	Rectangle* screenBoundry;
	QuadTree* quadTree;
	int totalEntityCount;
	int processedEntities;
	int m_processesPerFrame;

	//Entity processing unit
	EntityProcessingUnit* entityProcessingUnit;

	//Main
	int m_windowWidth;
	int m_windowHeight;

	//Draw quad tree or not. Gotten from data
	bool drawQuadTree;

	//Pause please
	bool pauseInGameScene;
	//Death please
	bool youLooseScene;

	//Play sounds
	void PlaySounds(FMOD::System* m_fModSystem, FMOD::ChannelGroup* effectsChannelGroup, FMOD::Channel* effectsChannel);
	bool playerPreviousJumped;
	bool previouslyInAir;

private:
	//UI
	UIElement* infoBar;

private:
	const int NUMBER_OF_DEATH_SOUNDS = 5;

	//Sounds
	FMOD::Sound* ingameMusic;
	FMOD::Sound* mingun;
	FMOD::Sound* gunShot;
	FMOD::Sound* shotGun;
	FMOD::Sound* shellsFalling;
	FMOD::Sound* walking;
	FMOD::Sound* reload;
	FMOD::Sound* grunt;
	FMOD::Sound* randomDead[5];

	//Music
	bool firstTimeToPlay;
};

#endif

