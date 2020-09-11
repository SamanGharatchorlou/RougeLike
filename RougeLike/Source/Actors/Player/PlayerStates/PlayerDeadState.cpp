#include "pch.h"
#include "PlayerDeadState.h"

#include "Actors/Player/Player.h"

#include "Audio/AudioManager.h"



PlayerDeadState::PlayerDeadState(Player* player) : mPlayer(player) { }


void PlayerDeadState::init()
{
	mPlayer->animator().selectAnimation(Animation::Dead);
	mPlayer->overrideControl(true);

	AudioManager::Get()->play("GameOver", mPlayer);

}


void PlayerDeadState::fastUpdate(float dt) { }


void PlayerDeadState::slowUpdate(float dt)
{
	if (mPlayer->animator().isLastFrame() && !mDeathTimer.isStarted())
	{
		mPlayer->animator().pause();
		mDeathTimer.start();

		AudioManager::Get()->play("Body Fall", mPlayer);
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