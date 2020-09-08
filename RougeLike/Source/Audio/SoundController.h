#pragma once

#include "Channel.h"


class Actor;

// 14 seems to be more than enough mixes channels
int constexpr mixerChannels = 14;
float constexpr maxVolume = (float)MIX_MAX_VOLUME;



class SoundController
{
public:
	SoundController();

	void clear();

	void init();
	void attenuationDistance(float maxDistance) { mAttenuationDistance = maxDistance * maxDistance; }
	void setListener(Actor* listener) { mListener = listener; }

	void slowUpdate();

	void playMusic(Audio* audio);
	void playSound(Audio* audio, uintptr_t id, VectorF source);
	void loopSound(Audio* audio, uintptr_t id, VectorF source);

	void pauseSound(Audio* audio, uintptr_t id);
	void resumeSound(Audio* audio, uintptr_t id);
	void stopSound(Audio* audio, uintptr_t id);

	void fadeOut(Audio* audio, uintptr_t id);

	bool hasActiveAudio(Audio* audio, uintptr_t id);
	bool isPlaying(Audio* audio, uintptr_t id);

	void setSoundVolume(float volume);
	float getSoundVolume() const { return soundVolume; }

	void setMusicVolume(float volume);
	float getMusicVolume() const { return musicVolume; }
	

private:
	void updateMixerVolume(Channel& channel);
	float attenuation(Channel& channel);

private:
	Channel channels[mixerChannels];

	float soundVolume;
	float musicVolume;

	Actor* mListener;
	float mAttenuationDistance;
};



