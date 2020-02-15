#include "pch.h"
#include "IdleState.h"

#include "Managers/GameController.h"
#include "Graphics/TextureManager.h"
#include "Input/InputManager.h"
#include "../Player.h"


IdleState::IdleState(GameData* gameData, Player* player)
{
	setupStateData(gameData, player);
	mState = PlayerState::Idle;
}


void IdleState::handleInput()
{
	/*
	Movement& movement = mPlayer->getMovement();
	InputManager& input = *mGameData->inputManager;

	if (input.isPressed(Button::UP)		||
		input.isPressed(Button::DOWN)	||
		input.isPressed(Button::RIGHT)	||
		input.isPressed(Button::LEFT))
	{
		changeState(ACTION_STATE::RUN);
	}
	else if (input.isCursorPressed())
	{
		addState(ACTION_STATE::ATTACK);
	}
	*/
}