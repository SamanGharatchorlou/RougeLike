#include "pch.h"
#include "SoundController.h"

#include "Audio.h"
#include "Actors/Actor.h"


SoundController::SoundController() : soundVolume(-1), musicVolume(-1)
{
	// Defualt to 8 channels
	Mix_AllocateChannels(MIX_CHANNELS);

	for (int i = 0; i < MIX_CHANNELS; i++)
	{
		channels[i].init(i);
	}

	float vol = getMusicVolume();
	int vim = Mix_VolumeMusic(-1);
}


void SoundController::init()
{
	setMusicVolume(musicVolume);
	for (int i = 0; i < MIX_CHANNELS; i++)
	{
		updateMixerVolume(channels[i]);
	}

	float vol = getMusicVolume();
	int vim = Mix_VolumeMusic(-1);
}


void SoundController::slowUpdate()
{
#if DEBUG_CHECK
	int counter = 0;
#endif
	float vol = getMusicVolume();
	int vim = Mix_VolumeMusic(-1);

	for (int i = 0; i < MIX_CHANNELS; i++)
	{
		Channel& sound = channels[i];

		if (sound.hasPlayingState() && sound.attenuate())
		{
			sound.mVolume = attenuation(sound);
		}

		if (sound.mState == Channel::FadingOut)
		{
			float volume = sound.mVolume * 0.98;
			sound.mVolume = volume;

			if (sound.mVolume < 0.02f)
			{
				sound.stop();
			}
		}
		else if (sound.mState == Channel::Looping)
		{
			if (!sound.isAudioRunning())
			{
				sound.playNext();
			}
		}

		// Free up channels after audio has stopped playing
		if (sound.canFree())
		{
			sound.free();
		}

		updateMixerVolume(sound);

		float vol = getMusicVolume();
		int vim = Mix_VolumeMusic(-1);

#if DEBUG_CHECK
		if (sound.mState != Channel::Free)
		{
			counter++;
		}
#endif
	}

#if DEBUG_CHECK // Check channels aren't getting clogged up with paused audio
	if (counter >= 7)
	{
		int playingCounter = 0;
		int pauseCounter = 0;

		for (int i = 0; i < MIX_CHANNELS; i++)
		{
			const Channel& channel = channels[i];
			if (channel.hasPlayingState())
				playingCounter++;
			else if (channel.isPaused())
				pauseCounter++;
		}

		DebugPrint(Warning, "7 or more out the the 8 audio channels are being used. %d channels are playing and %d channels are paused\n", playingCounter, pauseCounter);
	}
#endif

}


bool SoundController::isPlaying(Audio* audio, void* souceId)
{
	for (int i = 0; i < MIX_CHANNELS; i++)
	{
		if (channels[i].isPlaying(audio, souceId))
			return true;
	}

	return false;
}


bool SoundController::hasActiveAudio(Audio* audio, void* souceId)
{
	for (int i = 0; i < MIX_CHANNELS; i++)
	{
		if (channels[i].has(audio, souceId))
			return true;
	}

	return false;
}


void SoundController::playMusic(Audio* music)
{
	music->play(-1);
}


void SoundController::playSound(Audio* audio, void* sourceId, VectorF position)
{
	// Check if the source is already playing this sound
	for (int i = 0; i < MIX_CHANNELS; i++)
	{
		Channel& channel = channels[i];
		if (channel.isPlaying(audio, sourceId))
		{
			channel.play();
			channel.mSource = position;
			return;
		}
	}

	// Find free channel
	for (int i = 0; i < MIX_CHANNELS; i++)
	{
		Channel& channel = channels[i];
		if (channel.mState == Channel::Free)
		{
#if DEBUG_CHECK
			if (channel.mAudio)
				DebugPrint(Warning, "This channel still has an audio attached, but its state is free. This is wrong!\n");
#endif

			channel.mID = sourceId;
			channel.mAudio = audio;
			channel.mSource = position;
			channel.play();
			return;
		}
	}

	DebugPrint(Warning, "No available channels to play audio\n");
}


void SoundController::loopSound(Audio* audio, void* sourceId, VectorF position)
{
	// Check if the source is already playing this sound
	for (int i = 0; i < MIX_CHANNELS; i++)
	{
		Channel& channel = channels[i];
		if (channel.isPlaying(audio, sourceId))
		{
			channel.mSource = position;
			return;
		}
	}

	// Find free channel
	for (int i = 0; i < MIX_CHANNELS; i++)
	{
		Channel& channel = channels[i];
		if (channel.mState == Channel::Free)
		{
			channel.mID = sourceId;
			channel.mAudio = audio;
			channel.mSource = position;
			channel.loop();
			return;
		}
	}

	DebugPrint(Warning, "No available channels to loop audio\n");
}


void SoundController::pauseSound(Audio* audio, void* sourceId)
{
	for (int i = 0; i < MIX_CHANNELS; i++)
	{
		Channel& channel = channels[i];
		if (channel.isPlaying(audio, sourceId))
		{
			channel.pause();
			return;
		}
	}

	DebugPrint(Warning, "Could not pause audio\n");
}


void SoundController::resumeSound(Audio* audio, void* sourceId)
{
	for (int i = 0; i < MIX_CHANNELS; i++)
	{
		Channel& channel = channels[i];
		if (channel.isPaused() && channel.has(audio, sourceId))
		{
			channel.resume();
			return;
		}
	}

	DebugPrint(Warning, "Could not resume audio\n");
}


void SoundController::stopSound(Audio* audio, void* sourceId)
{
	for (int i = 0; i < MIX_CHANNELS; i++)
	{
		Channel& channel = channels[i];
		if (channel.has(audio, sourceId))
		{
			channel.stop();
			return;
		}
	}

	DebugPrint(Warning, "Could not stop audio,\n");
}


void SoundController::fadeOut(Audio* audio, void* sourceId)
{
	for (int i = 0; i < MIX_CHANNELS; i++)
	{
		Channel& channel = channels[i];
		if (channel.isPlaying(audio, sourceId))
		{
			channel.mState = Channel::FadingOut;
			return;
		}
	}


	DebugPrint(Warning, "Could not fade out audio\n");
}


void SoundController::setSoundVolume(float volume)
{ 
	soundVolume = volume;
}

void SoundController::setMusicVolume(float volume)
{
	musicVolume = volume <= 0.05 ? 0 : volume;
	float mixVolume = maxVolume * musicVolume;
	Mix_VolumeMusic((int)mixVolume);
}


// -- Private Functions -- //

void SoundController::updateMixerVolume(Channel& channel)
{
	float relativeVolume = soundVolume * channel.mVolume * maxVolume;
	Mix_Volume(channel.index(), (int)relativeVolume);
}

float SoundController::attenuation(Channel& channel)
{
	float audioDistance = distanceSquared(mListener->position(), channel.mSource);
	float attenuation = (mAttenuationDistance - audioDistance) / mAttenuationDistance;
	return clamp(attenuation, 0.0f, 1.0f);
}