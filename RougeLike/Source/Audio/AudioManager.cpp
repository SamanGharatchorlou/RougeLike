#include "pch.h"
#include "AudioManager.h"
#include "Audio.h"

AudioManager::AudioManager()
{
	DebugPrint(Log, "Audio manager created\n");
}

void AudioManager::init()
{
	DebugPrint(Log, "\n--- Loading Audio ---\n");
	int fails = 0;

	DebugPrint(Log, "\nBackground Music\n");
	fails += loadAllMUS(FileManager::Audio_Bg);

	DebugPrint(Log, "\nAudio Effects\n");
	fails += loadAllWAV(FileManager::Audio_Effects_Attack);
	fails += loadAllWAV(FileManager::Audio_Effects_GetHit);
	fails += loadAllWAV(FileManager::Audio_Effects_Physics);

	DebugPrint(Log, "\n--- Audio Loading Complete: %d Failures ---\n\n", fails);
}


int AudioManager::loadAllMUS(FileManager::Folder folder)
{
	int fails = 0;
	std::vector<std::string> imagePaths = FileManager::Get()->fullPathsInFolder(folder);

	for (const std::string& path : imagePaths)
	{
		fails += !loadMUS(FileManager::Get()->fileName(path), path);
	}

	return fails;
}

int AudioManager::loadAllWAV(FileManager::Folder folder)
{
	int fails = 0;
	std::vector<std::string> imagePaths = FileManager::Get()->fullPathsInFolder(folder);

	for (const std::string& path : imagePaths)
	{
		fails += !loadWav(FileManager::Get()->fileName(path), path);
	}

	return fails;
}


bool AudioManager::loadWav(const std::string& name, const std::string& filePath)
{
	Audio *audioWav = new Audio;

	if (audioWav->loadWav(filePath))
	{
		AudioClips[name] = audioWav;
		DebugPrint(Log, "Successfully loaded audio '%s' at %s\n", name.c_str(), filePath.c_str());
		return true;
	}
	else
	{
		DebugPrint(Warning, "Failure: audio NOT loaded '%s' at %s\n. SDL_mixer Error: %s\n", name.c_str(), filePath.c_str(), Mix_GetError());
		return false;
	}
}


bool AudioManager::loadMUS(const std::string& name, const std::string& filePath)
{
	Audio *audioMus = new Audio;

	if (audioMus->loadMUS(filePath))
	{
		AudioClips[name] = audioMus;
		DebugPrint(Log, "Successfully loaded music '%s' at %s\n", name.c_str(), filePath.c_str());
		return true;
	}
	else
	{
		DebugPrint(Warning, "Failure: music NOT loaded '%s' at %s\n. SDL_mixer Error: %s\n", name.c_str(), filePath.c_str(), Mix_GetError());
		return false;
	}
}


// what if I put an invalid texture path???
Audio* AudioManager::getAudio(const std::string& label) const
{
	auto search = AudioClips.find(label);

	if (search != AudioClips.end())
	{
		return search->second;
	}
	else
	{
		DebugPrint(Warning, "No item in audio map with label: %s\n", label.c_str());
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


void AudioManager::play(const std::string& label)
{
	Audio* audio = getAudio(label);
	audio->play();
}

bool AudioManager::isPlaying(const std::string& label) const
{
	Audio* audio = getAudio(label);
	return audio->isPlaying();
}

void AudioManager::stop(const std::string& label)
{
	getAudio(label)->stop();
}



