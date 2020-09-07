#include "pch.h"
#include "Channel.h"

#include "Audio.h"


Channel::Channel() : mIndex(-1)
{
	free();
}


bool Channel::hasPlayingState() const
{
	return mState == Playing || mState == Looping || mState == FadingOut;
}


bool Channel::attenuate() const
{
	return mSource.isPositive();
}


void Channel::free()
{
	mState = Free;
	mAudio = nullptr;
	mID = nullptr;
	mSource = VectorF(-1.0f, -1.0f);
	mVolume = 1.0f;
}


bool Channel::isPaused() const
{
	return mState == Paused;
}

// continue loop
void Channel::playNext()
{
	mAudio->playNext(mIndex);
}

// start loop
void Channel::loop()
{
	mState = Looping;
	mAudio->play(mIndex);
}

void Channel::play()
{
	mState = Playing;
	mAudio->play(mIndex);
}

void Channel::stop()
{
	mAudio->stop(mIndex);
}

void Channel::resume()
{
	mState = mPrePauseState;
	mPrePauseState = Free;
	mAudio->resume(mIndex);
}

void Channel::pause()
{
	mPrePauseState = mState;
	mState = Paused;
	mAudio->pause(mIndex);
}

bool Channel::isAudioRunning() const
{
	return mAudio->isPlaying(mIndex);
}


bool Channel::isPlaying(Audio* audio, void* sourceId) const
{
	return hasPlayingState() && has(audio, sourceId);
}

bool Channel::has(Audio* audio, void* sourceId) const
{
	return mID == sourceId && mAudio == audio;
}

bool Channel::canFree() const
{
	return hasPlayingState() && !isAudioRunning();
}