#include "pch.h"
#include "Audio.h"

// --- Sound --- //
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