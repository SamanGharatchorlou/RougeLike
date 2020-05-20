#include "pch.h"
#include "SoundController.h"

#include "Audio.h"


SoundController::SoundController() : gameVolume(-1.0f), musicVolume(-1.0f)
{
	// Defualt to 8 channels
	Mix_AllocateChannels(MIX_CHANNELS);

	for (int i = 0; i < MIX_CHANNELS; i++)
	{
		channels[i] = Free;
		sourceIds[i] = nullptr;
		playingAudio[i] = nullptr;
	}
}


void SoundController::slowUpdate()
{
#if _DEBUG
	int counter = 0;
#endif

	for (int i = 0; i < MIX_CHANNELS; i++)
	{
		// Free up channels after audio has stopped playing
		if (channels[i] == Playing && !Mix_Playing(i))
		{
			channels[i] = Free;
			sourceIds[i] = nullptr;
			playingAudio[i] = nullptr;
		}

#if _DEBUG
		if (channels[i] != Free)
		{
			counter++;
		}

		if (channels[i] != Paused)
		{
			pauseTimers[i].stop();
		}
#endif
	}

#if _DEBUG // Check channels aren't getting clogged up with paused audio
	if (counter >= 7)
	{
		int playingCounter = 0;
		int pauseCounter = 0;

		for (int i = 0; i < MIX_CHANNELS; i++)
		{
			if (channels[i] == Playing)
				playingCounter++;
			else if (channels[i] == Paused)
				pauseCounter++;
		}

		DebugPrint(Log, "7 or more out the the 8 audio channels are being used. %d channels are playing and %d channels are paused\n", playingCounter, pauseCounter);
	}
#endif

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
		if (channels[i] == Playing)
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
		if (channels[i] == Free)
		{
			audio->play(i);

			channels[i] = Playing;
			sourceIds[i] = sourceId;
			playingAudio[i] = audio;
			return;
		}
	}

	DebugPrint(Warning, "No available channels to play audio\n");
}


void SoundController::pauseSound(Audio* audio, void* sourceId)
{
	for (int i = 0; i < MIX_CHANNELS; i++)
	{
		if (channels[i] == Playing && playingAudio[i] == audio && sourceIds[i] == sourceId)
		{
			Mix_Pause(i);
			channels[i] = Paused;
			printf("pausing sound\n");
#if _DEBUG
			pauseTimers[i].start();
#endif

			return;
		}
	}

	DebugPrint(Warning, "Could not pause audio, place a breakpoint here to find out why\n");
}


void SoundController::resumeSound(Audio* audio, void* sourceId)
{
	for (int i = 0; i < MIX_CHANNELS; i++)
	{
		if (playingAudio[i] == audio && sourceIds[i] == sourceId)
		{
			if (channels[i] == Paused)
			{
				Mix_Resume(i);
				channels[i] = Playing;
				return;
			}
			else if (channels[i] == Playing)
			{
				// Do nothing
				return;
			}
		}
	}

	DebugPrint(Warning, "Could not resume audio, place a breakpoint here to find out why\n");
}


void SoundController::stopSound(Audio* audio, void* sourceId)
{
	for (int i = 0; i < MIX_CHANNELS; i++)
	{
		if (channels[i] >= Playing && playingAudio[i] == audio && sourceIds[i] == sourceId)
		{
			Mix_HaltChannel(i);

			channels[i] = Free;
			sourceIds[i] = nullptr;
			playingAudio[i] = nullptr;
			return;
		}
	}


	DebugPrint(Warning, "Could not stop audio, place a breakpoint here to find out why\n");
}


bool SoundController::isPlaying(Audio* audio, void* sourceId)
{
	for (int i = 0; i < MIX_CHANNELS; i++)
	{
		if (channels[i] == Playing && playingAudio[i] == audio && sourceIds[i] == sourceId)
		{
			return true;
		}
	}

	return false;
}


void SoundController::setSoundVolume(float volume)
{
	volume = clamp(volume, 0.0f, 1.0f);
	int mixVolume = (int)(MIX_MAX_VOLUME * volume);
	Mix_Volume(-1, mixVolume);
}


float SoundController::getSoundVolume() const
{
	return (float)Mix_Volume(-1, -1) / (float)MIX_MAX_VOLUME;
}


void SoundController::setMusicVolume(float volume)
{
	volume = clamp(volume, 0.0f, 1.0f);
	int mixVolume = (int)(MIX_MAX_VOLUME * volume);
	Mix_VolumeMusic(mixVolume);
}


float SoundController::getMusicVolume() const
{
	return (float)Mix_VolumeMusic(-1) / (float)MIX_MAX_VOLUME;
}


// also need to work this for music??
void SoundController::toggleMute()
{
	// Audio on
	if (Mix_VolumeMusic(-1) == 0 && Mix_Volume(-1, -1) == 0)
	{
		Mix_Volume(-1, gameVolume);
		//MixVol
	}
	// Audio off
	else
	{ 
		gameVolume = Mix_Volume(-1, -1);
		Mix_Volume(-1, 0);
	}
}