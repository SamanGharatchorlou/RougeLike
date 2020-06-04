#include "pch.h"
#include "Audio.h"

// --- Sound --- //

Sound::~Sound()
{
	Mix_FreeChunk(mChunk);
	mChunk = nullptr;
}


bool Sound::load(const std::string& filePath)
{
	mChunk = Mix_LoadWAV(filePath.c_str());

	if (!mChunk)
	{
		DebugPrint(Warning, "Failed to load sound audio file %s. SDL_mixer Error: %s\n", filePath, Mix_GetError());
		return false;
	}
	else
	{
		return true;
	}
}

void Sound::play(int channel)
{
	Mix_PlayChannel(channel, mChunk, 0);
}


// --- Music --- //

Music::~Music()
{
	Mix_FreeMusic(mMusic);
	mMusic = nullptr;
}


bool Music::load(const std::string& filePath)
{
	mMusic = Mix_LoadMUS(filePath.c_str());

	if (!mMusic)
	{
		DebugPrint(Warning, "Failed to load music audio file %s. SDL_mixer Error: %s\n", filePath, Mix_GetError());
		return false;
	}
	else
	{
		return true;
	}
}


void Music::play(int channel)
{
	if (!Mix_PlayingMusic())
	{
		Mix_PlayMusic(mMusic, -1);
	}
	else if (Mix_PausedMusic() == 1)
	{
		Mix_ResumeMusic();
	}
}


void Music::pause(int channel)
{
	if (Mix_PlayingMusic() == 1)
	{
		Mix_PauseMusic();
	}
}


void Music::stop(int channel)
{
	Mix_HaltMusic();
}



// --- Audio Group --- //
bool AudioGroup::load(const std::string& directoryPath)
{
	std::string groupName = FileManager::Get()->getItemName(directoryPath);
	std::vector<std::string> audioFilePaths = FileManager::Get()->fullPathsInFolder(directoryPath);

	for (int i = 0; i < audioFilePaths.size(); i++)
	{
		Audio* audio = new Sound;

		if (audio->load(audioFilePaths[i]))
		{
			group.push_back(audio);
		}
		else
		{
			DebugPrint(Warning, "Failure: sound '%s' NOT loaded into group '%s' in directory '%s'\n. SDL_mixer Error: %s\n",
				FileManager::Get()->getItemName(audioFilePaths[i]), groupName.c_str(), directoryPath.c_str(), Mix_GetError());
		}
	}

	DebugPrint(Log, "Successfully loaded %d/%d audio files into group '%s'\n", group.size(), audioFilePaths.size(), groupName.c_str());
	return (bool)group.size();
}


void AudioGroup::play(int channel)
{
	int randomNumber = randomNumberBetween(0, group.size());
	group[randomNumber]->play(channel);
}


