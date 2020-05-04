#include "pch.h"
#include "SoundController.h"

#include "Audio.h"


SoundController::SoundController()
{
	for (int i = 0; i < MIX_CHANNELS; i++)
	{
		channels[i] = false;
		sourceIds[i] = nullptr;
		playingAudio[i] = nullptr;
	}
}


void SoundController::slowUpdate()
{
	for (int i = 0; i < MIX_CHANNELS; i++)
	{
		if (channels[i] == true && !Mix_Playing(i))
		{
			channels[i] = false;
			sourceIds[i] = nullptr;
			playingAudio[i] = nullptr;
		}
	}
}


void SoundController::playMusic(Audio* music)
{
	music->play(-1);
}


void SoundController::playSound(Audio* audio, void* sourceId)
{
	// Check if the source is already playing this sound
	for (int i = 0; i < MIX_CHANNELS; i++)
	{
		if (channels[i] == true)
		{
			// This source is already playing, replay on same channel
			if (sourceIds[i] == sourceId && playingAudio[i] == audio)
			{
				audio->play(i);
				return;
			}
		}
	}

	// Find free channel
	for (int i = 0; i < MIX_CHANNELS; i++)
	{
		if (channels[i] == false)
		{
			audio->play(i);

			channels[i] = true;
			sourceIds[i] = sourceId;
			playingAudio[i] = audio;

			// TEMP
			int counter = 0;
			for (int i = 0; i < MIX_CHANNELS; i++)
			{
				if (channels[i] == true)
				{
					counter++;
				}
			}
			printf("%d channels are being used\n", counter);

			return;
		}
	}

	DebugPrint(Warning, "No available channels to play audio\n");
}



void SoundController::stopSound(Audio* audio, void* sourceId)
{
	for (int i = 0; i < MIX_CHANNELS; i++)
	{
		if (channels[i] == true && playingAudio[i] == audio && sourceIds[i] == sourceId)
		{
			Mix_HaltChannel(i);

			channels[i] = false;
			sourceIds[i] = nullptr;
			playingAudio[i] = nullptr;
			return;
		}
	}
}


bool SoundController::isPlaying(Audio* audio, void* sourceId)
{
	for (int i = 0; i < MIX_CHANNELS; i++)
	{
		if (channels[i] == true && playingAudio[i] == audio && sourceIds[i] == sourceId)
		{
			return true;
		}
	}

	return false;
}