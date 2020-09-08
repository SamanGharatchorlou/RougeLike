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
	mID = NULL;
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
#if PRINT_PLAYING_AUDIO
	DebugPrint(Log, "Looping audio: %s\n", mAudio->name().c_str());
#endif
}

void Channel::play()
{
	mState = Playing;
	mAudio->play(mIndex);
#if PRINT_PLAYING_AUDIO
	DebugPrint(Log, "Playing audio: %s\n", mAudio->name().c_str());
#endif
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


bool Channel::isPlaying(Audio* audio, uintptr_t id) const
{
	return hasPlayingState() && has(audio, id);
}

bool Channel::has(Audio* audio, uintptr_t id) const
{
	return mID == id && mAudio == audio;
}

bool Channel::canFree() const
{
	return hasPlayingState() && !isAudioRunning();
}