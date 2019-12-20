#ifndef __SOUND_MANAGER_H__
#define __SOUND_MANAGER_H__

#include "fmod.hpp"
#include "fmod_errors.h"
#include <string>
#include <map>

class IniParser;

class SoundManager
{
public:
	SoundManager();
	~SoundManager();
	void Initialise(IniParser* iniFile);
	void PlaySound(std::string name, bool effect);

protected:
	void UnloadFMOD();
	void AddSound(std::string name, bool looping);
	void FMODErrorCheck(FMOD_RESULT result);

protected:
	IniParser* m_pIniFile;

	FMOD::System* m_pFmodSystem;
	FMOD::ChannelGroup* m_pMusicChannelGroup, *m_pEffectsChannelGroup;
	FMOD::Channel* m_pMusicChannel, *m_pEffectsChannel;

	std::string m_soundPath;
	std::map<std::string, FMOD::Sound*> m_pLoadedSounds;
	std::map<std::string, FMOD::Sound*>::iterator iter;

	float m_EffectsVolume;
	float m_MusicVolume;
};

#endif
