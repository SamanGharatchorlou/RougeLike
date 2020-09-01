#include "pch.h"
#include "SoundController.h"

#include "Audio.h"

#if PRINT_PLAY_AUDIO
#endif


SoundController::SoundController() : gameVolume(-1), musicVolume(-1), muted(false)
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
		else if (channels[i] == FadingOut)
		{
			// TODO: this will depend on framerate
			// change to using a timer
			float volume = (float)Mix_Volume(i, -1) * 0.96f;

			Mix_Volume(i, (int)volume);

			if (Mix_Volume(i, -1) < 3)
			{
				Mix_Volume(i, Mix_Volume(-1, -1));
				stopSound(playingAudio[i], sourceIds[i]);
			}
		}
		else if (channels[i] == Looping)
		{
			if (!Mix_Playing(i))
			{
				playingAudio[i]->playNext(i);
			}
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


void SoundController::loopSound(Audio* audio, void* sourceId)
{
	// Check if the source is already playing this sound
	for (int i = 0; i < MIX_CHANNELS; i++)
	{
		if (channels[i] == Playing || channels[i] == Looping)
		{
			// This source is already playing, do nothing
			if (sourceIds[i] == sourceId && playingAudio[i] == audio)
			{
				return;
			}
		}
	}

	// Find free channel
	for (int i = 0; i < MIX_CHANNELS; i++)
	{
		if (channels[i] == Free)
		{
			channels[i] = Looping;
			sourceIds[i] = sourceId;
			playingAudio[i] = audio;
			return;
		}
	}

	DebugPrint(Warning, "No available channels to loop audio\n");
}


void SoundController::pauseSound(Audio* audio, void* sourceId)
{
	for (int i = 0; i < MIX_CHANNELS; i++)
	{
		if (channels[i] == Playing && playingAudio[i] == audio && sourceIds[i] == sourceId)
		{
			Mix_Pause(i);
			channels[i] = Paused;
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
			audio->stop(i);

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
		if ((channels[i] == Playing || channels[i] == Looping) && playingAudio[i] == audio && sourceIds[i] == sourceId)
		{
			return true;
		}
	}

	return false;
}


void SoundController::fadeOut(Audio* audio, void* sourceId)
{
	for (int i = 0; i < MIX_CHANNELS; i++)
	{
		if (channels[i] >= Playing && playingAudio[i] == audio && sourceIds[i] == sourceId)
		{
			channels[i] = FadingOut;
			return;
		}
	}


	DebugPrint(Warning, "Could not fade out audio, place a breakpoint here to find out why\n");
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


void SoundController::mute(bool shouldMute)
{
	// Audio on
	if (!shouldMute)
	{
		Mix_Volume(-1, gameVolume);
		Mix_VolumeMusic(musicVolume);

		muted = false;
	}
	// Audio off
	else
	{
		gameVolume = Mix_Volume(-1, -1);
		musicVolume = Mix_VolumeMusic(-1);

		Mix_Volume(-1, 0);
		Mix_VolumeMusic(0);

		muted = true;
	}
}