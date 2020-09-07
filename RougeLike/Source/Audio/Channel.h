#pragma once

class Audio;

class Channel
{
public:
	enum State
	{
		Free,
		Playing,
		Looping,
		Paused,
		FadingOut
	};

public:
	Channel();
	void init(int index) { mIndex = index; }

	void play();
	void loop();
	void playNext();
	void stop();

	void resume();
	void pause();
	bool isPaused() const;

	bool hasPlayingState() const;
	bool isAudioRunning() const;

	bool has(Audio* audio, void* sourceId) const;
	bool isPlaying(Audio* audio, void* sourceId) const;

	bool attenuate() const;
	void setVolume(float volume) { mVolume = volume; }

	bool canFree() const;
	void free();

	void setIndex(int index) { mIndex = index; }
	int index() const { return mIndex; }

public:
	State mState;
	Audio* mAudio;
	void* mID;
	VectorF mSource;

	// 0 - 1 independant of game sound volume
	float mVolume;

	State mPrePauseState;

private:
	int mIndex;
};