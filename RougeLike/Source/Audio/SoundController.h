#pragma once

class Audio;

class SoundController
{
public:
	SoundController();

	void slowUpdate();

	void playMusic(Audio* audio);
	void playSound(Audio* audio, void* sourceId);

	void stopSound(Audio* audio, void* sourceId);

	bool isPlaying(Audio* audio, void* sourceId);

private:
	// Playing channels
	bool channels[MIX_CHANNELS];

	// Source of the audio
	void* sourceIds[MIX_CHANNELS];

	// Audio file
	Audio* playingAudio[MIX_CHANNELS];
};