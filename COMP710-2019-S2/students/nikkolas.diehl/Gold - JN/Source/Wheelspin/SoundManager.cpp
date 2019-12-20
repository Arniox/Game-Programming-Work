#include "SoundManager.h"

#include "IniParser.h"
#include "game.h"
#include "logmanager.h"

SoundManager::SoundManager()
: m_pIniFile()
{
}

SoundManager::~SoundManager()
{
	m_pIniFile = nullptr;

	UnloadFMOD();
}

void
SoundManager::Initialise(IniParser* iniFile)
{
	m_pIniFile = iniFile;

	m_soundPath = m_pIniFile->GetValueAsString("Sound", "soundPath");

	FMOD::System_Create(&m_pFmodSystem);
	m_pFmodSystem->init(32, FMOD_INIT_NORMAL, 0);

	m_EffectsVolume = m_pIniFile->GetValueAsFloat("Sound and Volume", "effectsVolume");
	m_MusicVolume = m_pIniFile->GetValueAsFloat("Sound and Volume", "musicVolume");

	FMODErrorCheck(m_pFmodSystem->createChannelGroup("Music Group", &m_pMusicChannelGroup));
	FMODErrorCheck(m_pFmodSystem->createChannelGroup("Effects Group", &m_pEffectsChannelGroup));
	
	m_pEffectsChannel->setChannelGroup(m_pEffectsChannelGroup);
	m_pMusicChannel->setChannelGroup(m_pMusicChannelGroup);
	
	m_pEffectsChannelGroup->setVolume(m_EffectsVolume);
	m_pMusicChannelGroup->setVolume(m_MusicVolume);

	//Sounds to add
	AddSound("clickNoise", false);
	AddSound("reloadNoise", false);
}

void 
SoundManager::PlaySound(std::string name, bool effect)
{
	if (m_pLoadedSounds.find(name) != m_pLoadedSounds.end())
	{
		if (effect) {
			FMODErrorCheck(m_pFmodSystem->playSound(m_pLoadedSounds[name], m_pEffectsChannelGroup, true, &m_pEffectsChannel));
		}
		else 
		{
			FMODErrorCheck(m_pFmodSystem->playSound(m_pLoadedSounds[name], m_pMusicChannelGroup, true, &m_pMusicChannel));
		}
	}
	else 
	{
		LogManager::Debug("ERROR - Sound could not be found");
	}

	m_pEffectsChannel->setChannelGroup(m_pEffectsChannelGroup);
	m_pMusicChannel->setChannelGroup(m_pMusicChannelGroup);

	if (effect)
	{
		m_pEffectsChannel->setPaused(false);
	}
	else 
	{
		m_pMusicChannel->setPaused(false);
	}
}

void 
SoundManager::AddSound(std::string name, bool looping)
{
	FMOD_MODE mode_nonLooping = FMOD_LOOP_OFF | FMOD_2D | FMOD_3D_WORLDRELATIVE | FMOD_3D_INVERSEROLLOFF;
	FMOD_MODE mode_looping = FMOD_LOOP_NORMAL | FMOD_2D | FMOD_3D_WORLDRELATIVE | FMOD_3D_INVERSEROLLOFF;

	FMOD::Sound* sound = nullptr;

	if (looping) 
	{
		m_pFmodSystem->createSound(std::string(m_soundPath + name + ".wav").c_str(), mode_looping, NULL, &sound);
		sound->setLoopCount(-1); //Infinite
	}
	else 
	{
		m_pFmodSystem->createSound(std::string(m_soundPath + name + ".wav").c_str(), mode_nonLooping, NULL, &sound);
	}

	m_pLoadedSounds.emplace(name, sound);
}

void 
SoundManager::UnloadFMOD()
{
	std::map<std::string, FMOD::Sound*>::iterator iter;
	iter = m_pLoadedSounds.begin();

	while (iter != m_pLoadedSounds.end())
	{
		(*iter).second->release();
		iter = m_pLoadedSounds.erase(iter);
	}

	m_pLoadedSounds.clear();

	m_pEffectsChannel = nullptr;
	m_pMusicChannel = nullptr;
	m_pMusicChannelGroup = nullptr;
	m_pEffectsChannelGroup = nullptr;
	m_pFmodSystem = nullptr;
}

void 
SoundManager::FMODErrorCheck(FMOD_RESULT result)
{
	if (result != FMOD_OK) 
	{
		const char* errorRes = FMOD_ErrorString(result);
		std::string outPut = "FMOD ERROR - ";
		outPut += errorRes;

		LogManager::Debug(outPut.c_str());
		Game::GetInstance()->Quit();
	}
	else {
		return;
	}
}
