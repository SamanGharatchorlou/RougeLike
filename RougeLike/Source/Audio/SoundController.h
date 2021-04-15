#pragma once

#include "Channel.h"


class Actor;

// 14 seems to be more than enough mixer channels
int constexpr mixerChannels = 8;
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

	void play(Audio* audio, uintptr_t id, VectorF source);
	void loop(Audio* audio, uintptr_t id, VectorF source);

	void pauseSound(Audio* audio, uintptr_t id);
	void resumeSound(Audio* audio, uintptr_t id);
	void stopSound(Audio* audio, uintptr_t id);

	void fadeIn(Audio* audio, uintptr_t id, int ms, VectorF source);
	void fadeInMusic(Audio* audio, uintptr_t id, int ms);
	void fadeOut(Audio* audio, uintptr_t id, int ms);

	bool hasActiveAudio(Audio* audio, uintptr_t id) const;
	bool isPlaying(Audio* audio, uintptr_t id) const;

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



