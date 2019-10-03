#define _CRTDBG_MAP_ALLOC
#include "SceneManager.h"
#include "InGameScene.h"
#include "SplashScreen.h"
#include "inputhandler.h"
#include "MainMenuScene.h"

SceneManager::SceneManager()
: currentState(SPLASH_SCREEN)
, splashScreenScene()
, mainMenuScene()
, instructionScene()
, inGameScene()
, pauseMenuScene()
, ingameVolume(0)
, menuVolume(0)
{
}

SceneManager::~SceneManager()
{
	//Empty sound
	UnloadFMOD();

	delete inGameScene;
	inGameScene = nullptr;
	delete splashScreenScene;
	splashScreenScene = nullptr;
	delete mainMenuScene;
	mainMenuScene = nullptr;
	delete instructionScene;
	instructionScene = nullptr;
	delete pauseMenuScene;
	pauseMenuScene = nullptr;
}

void SceneManager::Initialise(IniParser* m_iniParser, BackBuffer* m_pBackBuffer, int screenWidth, int screenHeight)
{
	//Copy references
	iniReferenceCopy = m_iniParser;
	bufferReferenceCopy = m_pBackBuffer;

	//Init Fmod
	InitFMOD(m_iniParser);

	splashScreenScene = new SplashScreen();
	splashScreenScene->Initialise(m_pBackBuffer, screenWidth, screenHeight);

	inGameScene = new InGameScene();
	inGameScene->Initialise(m_fmodSystem, m_iniParser, m_pBackBuffer, screenWidth, screenHeight);

	pauseMenuScene = new PauseMenu();
	pauseMenuScene->Initialise(m_iniParser, m_pBackBuffer, screenWidth, screenHeight);

	mainMenuScene = new MainMenuScene();
	mainMenuScene->Initialise(m_pBackBuffer, screenWidth, screenHeight);

	instructionScene = new InstructionScene();
	instructionScene->Initialise(m_iniParser, m_pBackBuffer, screenWidth, screenHeight);
}

void
SceneManager::Process(InputHandler* inputHandler, float deltaTime, int screenWidth, int screenHeight)
{
	switch (currentState) {
	case(SPLASH_SCREEN):
		splashScreenTime += deltaTime;
		if (static_cast<int>(splashScreenTime) >= SplashMax) {
			ChangeState(MAIN_MENU);
		}
		break;
	case(MAIN_MENU):
		mainMenuScene->Process(inputHandler, deltaTime);
		//Request play sound
		PlayClick(mainMenuScene->RequestSound());

		if (mainMenuScene->RequestPlay()) {
			PlayActivate();
			ChangeState(INGAME);
		}
		if (mainMenuScene->RequestInstruction()) {
			PlayActivate();
			ChangeState(INSTRUCTION);
		}
		break;
	case(INSTRUCTION):
		instructionScene->Process(inputHandler, deltaTime);
		//Request play sound
		PlayClick(instructionScene->RequestSound());

		if (instructionScene->RequestBack()) {
			PlayActivate();
			ChangeState(MAIN_MENU);
		}
		break;
	case(INGAME):
		if (!inGameScene->RequestPause() && !inGameScene->RequestFailure()) {
			inGameScene->Process(m_fmodSystem, m_ingameGroup, m_ingameEffects, inputHandler, deltaTime, screenWidth, screenHeight);
			//Play ingame music
			inGameScene->PlayInGameMusic(*m_fmodSystem, m_ingameGroup, m_ingameMusic);
			m_ingameGroup->setPaused(false);
		}
		else {
			//Pause ingame music
			m_ingameGroup->setPaused(true);

			//Set loose screen or not
			pauseMenuScene->SetLoose(inGameScene->RequestFailure());

			pauseMenuScene->Process(inputHandler, deltaTime);
			//Request play sound
			PlayClick(pauseMenuScene->RequestSound());

			if (pauseMenuScene->RequestBack()) {
				PlayActivate();

				//Delete and reinitiate the entire ingame
				delete inGameScene;
				inGameScene = nullptr;
				//Re-create
				inGameScene = new InGameScene();
				inGameScene->Initialise(m_fmodSystem, iniReferenceCopy, bufferReferenceCopy, screenWidth, screenHeight);

				ChangeState(MAIN_MENU);
			}
			if (pauseMenuScene->RequestContinue()) {
				PlayActivate();
				inGameScene->UnPause();
			}
		}
		break;
	}

	//Set volumes
	m_menuGroup->setVolume(menuVolume);
	m_ingameGroup->setVolume(ingameVolume);

	//Process sound
	m_fmodSystem->update();
}

int 
SceneManager::GetProcessingCount()
{
	return inGameScene->GetProcessingCount();
}

int 
SceneManager::GetTotalEntities()
{
	return inGameScene->GetTotalEntities();
}

int 
SceneManager::GetProcessedEntities()
{
	return inGameScene->GetEntitiesProcessed();
}

void
SceneManager::Draw(BackBuffer& m_backBuffer, int& m_FPS, int& m_totalFramesPassed)
{
	switch (currentState) {
		case(SPLASH_SCREEN):
			splashScreenScene->Draw(m_backBuffer);
			break;
		case(MAIN_MENU):
			mainMenuScene->Draw(m_backBuffer);
			break;
		case(INSTRUCTION):
			instructionScene->Draw(m_backBuffer);
			break;
		case(INGAME):
			inGameScene->Draw(m_backBuffer, m_FPS, m_totalFramesPassed);
			//Draw the pause menu overtop the game
			if (inGameScene->RequestPause() || inGameScene->RequestFailure()) {
				pauseMenuScene->Draw(m_backBuffer);
			}
			break;
	}
}

void 
SceneManager::ChangeState(State stateToChange)
{
	currentState = stateToChange;

	switch (currentState) {
		case(MAIN_MENU):
			mainMenuScene->selected = -1;
			break;
		case(INSTRUCTION):
			instructionScene->selected = -1;
			break;
		case(INGAME):
			pauseMenuScene->selected = -1;
			break;
	}
}

void
SceneManager::InitFMOD(IniParser* m_iniParser)
{
	FMOD::System_Create(&m_fmodSystem);
	m_fmodSystem->init(32, FMOD_INIT_NORMAL, 0);

	m_fmodSystem->createChannelGroup(NULL, &m_menuGroup);
	m_fmodSystem->createChannelGroup(NULL, &m_ingameGroup);

	m_menuSounds->setChannelGroup(m_menuGroup);
	m_ingameMusic->setChannelGroup(m_ingameGroup);
	m_ingameEffects->setChannelGroup(m_ingameGroup);

	//Volume and sound
	ingameVolume = static_cast<float>(m_iniParser->GetValueAsDouble("Sound and Volume", "ingameVolume"));
	menuVolume = static_cast<float>(m_iniParser->GetValueAsDouble("Sound and Volume", "menuVolume"));

	std::string clickS = m_iniParser->GetValueAsString("AllMenus", "click");
	std::string activateS = m_iniParser->GetValueAsString("AllMenus", "activate");
	m_fmodSystem->createSound(clickS.c_str(), FMOD_DEFAULT, NULL, &clickNoise);
	m_fmodSystem->createSound(activateS.c_str(), FMOD_DEFAULT, NULL, &activateNoise);
}

void
SceneManager::UnloadFMOD()
{
	//Clear the channels
	m_menuSounds = nullptr;
	m_ingameMusic = nullptr;
	m_ingameEffects = nullptr;
	//Clear the channel groups
	m_menuGroup = nullptr;
	m_ingameGroup = nullptr;
	//Clear the system
	m_fmodSystem = nullptr;
	//Release all sounds
	clickNoise->release();
	activateNoise->release();
}

void 
SceneManager::PlayClick(bool didRequest)
{
	if (didRequest) {
		m_fmodSystem->playSound(clickNoise, m_menuGroup, false, &m_menuSounds);
	}
}

void 
SceneManager::PlayActivate()
{
	m_fmodSystem->playSound(activateNoise, m_menuGroup, false, &m_menuSounds);
}