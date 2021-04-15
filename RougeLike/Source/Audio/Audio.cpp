#include "pch.h"
#include "Audio.h"


// --- Sound --- //
Sound::~Sound()
{
	Mix_FreeChunk(mChunk);
}


bool Sound::load(const BasicString& filePath)
{
	mChunk = Mix_LoadWAV(filePath.c_str());

	if (!mChunk)
	{
		DebugPrint(Warning, "Failed to load sound audio file %s. SDL_mixer Error: %s\n", filePath.c_str(), Mix_GetError());
		return false;
	}
	else
	{
#if PRINT_PLAYING_AUDIO
		mFilePath = filePath;
#endif
		return true;
	}
}

void Sound::play(int channel)
{
	Mix_PlayChannel(channel, mChunk, 0);
}

void Sound::stop(int channel)
{
	Mix_HaltChannel(channel);
}

bool Sound::isPlaying(int channel) const
{
	return Mix_Playing(channel);
}

void Sound::pause(int channel)
{
	Mix_Pause(channel);
}

void Sound::resume(int channel)
{
	Mix_Resume(channel);
}


void Sound::fadeIn(int channel, int ms)
{
	Mix_FadeInChannel(channel, mChunk, 0, ms);
}

void Sound::fadeOut(int channel, int ms)
{
	Mix_FadeOutChannel(channel, ms);
}


// --- Music --- //

Music::~Music()
{
	Mix_FreeMusic(mMusic);
}


bool Music::load(const BasicString& filePath)
{
	mMusic = Mix_LoadMUS(filePath.c_str());

	if (!mMusic)
	{
		DebugPrint(Warning, "Failed to load music audio file %s. SDL_mixer Error: %s\n", filePath, Mix_GetError());
		return false;
	}
	else
	{
#if PRINT_PLAYING_AUDIO
		mFilePath = filePath;
#endif
		return true;
	}
}


void Music::play(int channel)
{
	Mix_PlayMusic(mMusic, -1);

#if PRINT_PLAY_AUDIO
	DebugPrint(Log, "Playing music %s\n", mFilePath.c_str());
#endif
}

void Music::resume(int channel)
{
	Mix_ResumeMusic();
}


void Music::pause(int channel)
{
	Mix_PauseMusic();
}


void Music::stop(int channel)
{
	Mix_HaltMusic();
}

bool Music::isPlaying(int channel) const
{
	return Mix_PlayingMusic();
}

void Music::fadeIn(int channel, int ms)
{
	Mix_FadeInMusic(mMusic, -1, ms);
}

void Music::fadeOut(int channel, int ms)
{
	Mix_FadeOutMusic(ms);
}



// --- Audio Group --- //
SoundGroup::~SoundGroup()
{
	for (int i = 0; i < group.size(); i++)
	{
		Audio* audio = group[i];
		delete audio;
	}
}



bool SoundGroup::load(const BasicString& directoryPath)
{
	BasicString groupName = FileManager::Get()->getItemName( directoryPath);
	std::vector<BasicString> audioFilePaths = FileManager::Get()->fullPathsInFolder(directoryPath);

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

#if PRINT_PLAYING_AUDIO
	if(group.size() > 0)
		mFilePath = audioFilePaths[0];
#endif

	DebugPrint(Log, "Successfully loaded %d/%d audio files into group '%s'\n", group.size(), audioFilePaths.size(), groupName.c_str());
	return (bool)group.size();
}


void SoundGroup::play(int channel)
{
	int randomNumber = randomNumberBetween(0, group.size());
	group[randomNumber]->play(channel);
}


void SoundGroup::playNext(int channel)
{
	playingIndex = playingIndex + 1 >= group.size() ? 0 : playingIndex + 1;
	group[playingIndex]->play(channel);
}

void SoundGroup::stop(int channel)
{
	playingIndex = 0;
	Mix_HaltChannel(channel);
}

bool SoundGroup::isPlaying(int channel) const
{
	return group[playingIndex]->isPlaying(channel);
}

void SoundGroup::pause(int channel)
{
	group[playingIndex]->pause(channel);
}

void SoundGroup::resume(int channel)
{
	group[playingIndex]->resume(channel);
}

void SoundGroup::fadeIn(int channel, int ms)
{
	int randomNumber = randomNumberBetween(0, group.size());
	group[randomNumber]->fadeIn(channel, ms);
}

void SoundGroup::fadeOut(int channel, int ms)
{
	group[playingIndex]->fadeOut(channel, ms);
}