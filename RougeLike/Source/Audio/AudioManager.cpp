#include "pch.h"
#include "AudioManager.h"
#include "Audio.h"

AudioManager::AudioManager()
{
	DebugPrint(Log, "Audio manager created\n");
}

void AudioManager::init()
{
	// load background music
	// load basic blaster clip
	DebugPrint(Log, "Audio manager initialised\n");
}



void AudioManager::loadWav(const std::string name, const std::string filePath)
{
	Audio *audioWav = new Audio;

	if (audioWav->loadWav(filePath))
	{
		AudioClips[name] = audioWav;
		DebugPrint(Log, "Successfully loaded audio %s file path %s\n", name.c_str(), filePath.c_str());
	}
	else
	{
		DebugPrint(Warning, "Failed to load audio file %s. SDL_mixer Error: %s\n", filePath, Mix_GetError());
	}
}


void AudioManager::loadMUS(const std::string name, const std::string filePath)
{
	Audio *audioMus = new Audio;

	if (audioMus->loadMUS(filePath))
	{
		AudioClips[name] = audioMus;
		DebugPrint(Log, "Successfully loaded music %s file path %s\n", name.c_str(), filePath.c_str());
	}
	else
	{
		DebugPrint(Warning, "Failed to load audio file %s. SDL_mixer Error: %s\n", filePath, Mix_GetError());
	}
}


// what if I put an invalid texture path???
Audio* AudioManager::getAudio(const std::string label) const
{
	auto search = AudioClips.find(label);

	if (search != AudioClips.end())
	{
		return search->second;
	}
	else
	{
		DebugPrint(Warning, "No item in audio clip map with label: %s\n", label);
		return nullptr;
	}
}

void AudioManager::toggleMute() const
{
	if (Mix_VolumeMusic(-1) == 0 && Mix_Volume(-1, -1) == 0)
	{
		Mix_VolumeMusic(volume);
		Mix_Volume(-1, volume);
		DebugPrint(Log, "Audio ON\n");
	}
	else
	{
		Mix_VolumeMusic(0);
		Mix_Volume(-1, 0);
		DebugPrint(Log, "Audio OFF\n");
	}
}

void AudioManager::pauseMusic() const
{
	for (auto iter = AudioClips.begin(); iter != AudioClips.end(); iter++)
	{
		if (iter->second->isMusic)
		{
			iter->second->pause();
			break;
		}
	}
}


void AudioManager::setVolume(int theVolume)
{
	volume = clamp(theVolume, 0, MIX_MAX_VOLUME);
	Mix_VolumeMusic(volume);
	Mix_Volume(-1, volume);
}
