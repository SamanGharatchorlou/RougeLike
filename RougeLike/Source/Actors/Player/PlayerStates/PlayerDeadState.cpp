#include "pch.h"
#include "PlayerDeadState.h"

#include "Actors/Player/Player.h"

#include "Audio/AudioManager.h"



PlayerDeadState::PlayerDeadState(Player* player) : mPlayer(player) { }


void PlayerDeadState::init()
{
	mPlayer->animator().selectAnimation(Action::Dead);
	mPlayer->overrideControl(true);
}


void PlayerDeadState::fastUpdate(float dt) { }


void PlayerDeadState::slowUpdate(float dt)
{
	if (mPlayer->animator().isLastFrame() && !mDeathTimer.isStarted())
	{
		mPlayer->animator().pause();
		mDeathTimer.start();
	}
}


void PlayerDeadState::exit()
{
	PlayerDeadEvent* deadEventPtr = new PlayerDeadEvent;
	EventPacket event(deadEventPtr);
	mPlayer->events().push(event);
}


bool PlayerDeadState::finished()
{
	return mDeathTimer.getSeconds() > 2.0f;
}