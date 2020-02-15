#include "pch.h"
#include "Audio.h"


bool Audio::loadWav(const std::string filePath)
{
	mixer.gScratch = Mix_LoadWAV(filePath.c_str());

	if (mixer.gScratch == NULL)
	{
		DebugPrint(Warning, "Failed to load audio file %s. SDL_mixer Error: %s\n", filePath, Mix_GetError());
		return false;
	}
	else
	{
		isMusic = false;
		return true;
	}
}

bool Audio::loadMUS(const std::string filePath)
{
	mixer.gMusic = Mix_LoadMUS(filePath.c_str());

	if (mixer.gScratch == NULL)
	{
		DebugPrint(Warning, "Failed to load audio file %s. SDL_mixer Error: %s\n", filePath, Mix_GetError());
		return false;
	}
	else
	{
		isMusic = true;
		return true;
	}
}

void Audio::play() const
{
	// MUSIC
	if (isMusic)
	{
		if (Mix_PlayingMusic() == 0)
		{
			//Play the music
			Mix_PlayMusic(mixer.gMusic, -1);
		}
		else if (Mix_PausedMusic() == 1)
		{
			Mix_ResumeMusic();
		}
	}

	// SOUND
	else
	{
		Mix_PlayChannel(-1, mixer.gScratch, 0);
	}
}

void Audio::pause() const
{
	if (isMusic)
	{
		if (Mix_PlayingMusic() == 1)
		{
			Mix_PauseMusic();
		}
	}
}

void Audio::stop() const
{
	if (isMusic)
	{
		Mix_HaltMusic();
	}
}