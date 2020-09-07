#pragma once

#include "Channel.h"


class Actor;

float constexpr maxVolume = (float)MIX_MAX_VOLUME;




class SoundController
{
public:
	SoundController();

	void init();
	void attenuationDistance(float maxDistance) { mAttenuationDistance = maxDistance * maxDistance; }
	void setListener(Actor* listener) { mListener = listener; }

	void slowUpdate();

	void playMusic(Audio* audio);
	void playSound(Audio* audio, void* sourceId, VectorF source);
	void loopSound(Audio* audio, void* sourceId, VectorF source);

	void pauseSound(Audio* audio, void* sourceId);
	void resumeSound(Audio* audio, void* sourceId);
	void stopSound(Audio* audio, void* sourceId);

	void fadeOut(Audio* audio, void* sourceId);

	bool hasActiveAudio(Audio* audio, void* souceId);
	bool isPlaying(Audio* audio, void* souceId);

	void setSoundVolume(float volume);
	float getSoundVolume() const { return soundVolume; }

	void setMusicVolume(float volume);
	float getMusicVolume() const { return musicVolume; }
	

private:
	void updateMixerVolume(Channel& channel);
	float attenuation(Channel& channel);

private:

	Channel channels[MIX_CHANNELS];

	float soundVolume;
	float musicVolume;

	Actor* mListener;
	float mAttenuationDistance;
};



