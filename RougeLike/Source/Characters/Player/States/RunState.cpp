#include "pch.h"
#include "RunState.h"

#include "Managers/GameController.h"
#include "Input/InputManager.h"
#include "../Player.h"

RunState::RunState(GameData* gameData, Player* player)
{
	setupStateData(gameData, player);
	mState = PlayerState::Run;
}


void RunState::handleInput()
{
	/*
	Movement& movement = mPlayer->getMovement();
	InputManager& input = *mGameData->inputManager;

	movement.resetDirection();

	// Held keys
	if (input.isHeld(Button::UP))
		movement.up();
	
	if (input.isHeld(Button::DOWN))
		movement.down();

	if (input.isHeld(Button::LEFT))
		movement.left();

	if (input.isHeld(Button::RIGHT))
		movement.right();
		*/
}


void RunState::update(float dt)
{
	// set new position
	mPlayer->getMovement().update(dt);
	VectorF position = mPlayer->getMovement().getPostion();
	mPlayer->getRect().SetTopLeft(position);

	if (!mPlayer->getMovement().isMoving())
	{
		//changeState(ACTION_STATE::IDLE);
	}
}

