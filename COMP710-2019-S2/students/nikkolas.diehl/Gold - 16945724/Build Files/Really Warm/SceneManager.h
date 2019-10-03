#ifndef __SCENEMANAGER_H__
#define __SCENEMANAGER_H__

//Locals
#include "fmod.hpp"
#include "iniParser.h"
#include "backbuffer.h"
#include "InGameScene.h"
#include "SplashScreen.h"
#include "inputhandler.h"
#include "MainMenuScene.h"
#include "InstructionScene.h"
#include "PauseMenu.h"

//Libraries
#include <vector>

class SceneManager
{
public:
	SceneManager();
	~SceneManager();

	//Draw and process
	void Initialise(IniParser* m_iniParser, BackBuffer* m_pBackBuffer, int screenWidth, int screenHeight);
	void Draw(BackBuffer& m_pBackBuffer, int& m_FPS, int& m_totalFramesPassed);
	void Process(InputHandler* inputHandler, float deltaTime, int screenWidth, int screenHeight);

	int GetProcessingCount();
	int GetTotalEntities();
	int GetProcessedEntities();

	//Game states
	enum State {
		SPLASH_SCREEN,
		MAIN_MENU,
		INSTRUCTION,
		INGAME
	};
	//Current state
	State currentState;
	//Change state functionality
	void ChangeState(State stateToChange);
	//Scenes
	InGameScene* inGameScene;
	PauseMenu* pauseMenuScene;
	SplashScreen* splashScreenScene;
	MainMenuScene* mainMenuScene;
	InstructionScene* instructionScene;

private:
	//Reference copies
	IniParser* iniReferenceCopy;
	BackBuffer* bufferReferenceCopy;

private:
	//Menu noises
	FMOD::Sound* clickNoise;
	FMOD::Sound* activateNoise;

	//Timing
	const int SplashMax = 3;
	double splashScreenTime = 0;

	//Fmod
	FMOD::System* m_fmodSystem;
	FMOD::ChannelGroup* m_menuGroup;
	FMOD::ChannelGroup* m_ingameGroup;

	FMOD::Channel* m_menuSounds;
	FMOD::Channel* m_ingameMusic;
	FMOD::Channel* m_ingameEffects;

	//Volumes
	float menuVolume;
	float ingameVolume;

	void InitFMOD(IniParser* m_iniParser);
	void UnloadFMOD();
	void PlayClick(bool didRequest);
	void PlayActivate();
};

#endif

