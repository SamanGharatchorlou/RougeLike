#pragma once

class Audio;

class SoundController
{
public:
	SoundController();

	void slowUpdate();

	void playMusic(Audio* audio);
	void playSound(Audio* audio, void* sourceId);

	void pauseSound(Audio* audio, void* sourceId);
	void resumeSound(Audio* audio, void* sourceId);
	void stopSound(Audio* audio, void* sourceId);

	bool isPlaying(Audio* audio, void* sourceId);

private:
	enum Channelstate
	{
		Free,
		Playing,
		Paused
	}; 

	// Playing channels
	Channelstate channels[MIX_CHANNELS];

	// Source of the audio
	void* sourceIds[MIX_CHANNELS];

	// Audio file
	Audio* playingAudio[MIX_CHANNELS];

#if _DEBUG
	Timer<float> pauseTimers[MIX_CHANNELS];
#endif
};