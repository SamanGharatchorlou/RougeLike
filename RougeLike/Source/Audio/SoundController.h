#pragma once

class Audio;

class SoundController
{
public:
	SoundController();

	void slowUpdate();

	void playMusic(Audio* audio);
	void playSound(Audio* audio, void* sourceId);
	void loopSound(Audio* audio, void* sourceId);

	void pauseSound(Audio* audio, void* sourceId);
	void resumeSound(Audio* audio, void* sourceId);
	void stopSound(Audio* audio, void* sourceId);

	void fadeOut(Audio* audio, void* sourceId);

	bool isPlaying(Audio* audio, void* sourceId);

	void setSoundVolume(float volume);
	float getSoundVolume() const;

	void setMusicVolume(float volume);
	float getMusicVolume() const;

	void mute(bool shouldMute);
	void toggleMute() { mute(!muted); }

private:
	enum Channelstate
	{
		Free,
		Playing,
		Looping,
		Paused,
		// TODO add fast/slow fades
		FadingOut
	}; 

	// Playing channels
	Channelstate channels[MIX_CHANNELS];

	// Source of the audio
	void* sourceIds[MIX_CHANNELS];

	// Audio file
	Audio* playingAudio[MIX_CHANNELS];

	int gameVolume;
	int musicVolume;
	bool muted;

#if DEBUG_CHECK
	Timer<float> pauseTimers[MIX_CHANNELS];
#endif
};