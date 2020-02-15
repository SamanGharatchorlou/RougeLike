#pragma once
#include "pch.h"

#include "PlayerState.h"
#include "../Player.h"

#include "Game/GameData.h"
#include "Graphics/Texture.h"


void PlayerState::setupStateData(GameData* gameData, Player* player)
{
	mGameData = gameData;
	mPlayer = player;
}


void PlayerState::changeState(actionState newState)
{
	DebugPrint(Log, "Unimplemented PlayerState::changeState()\n");
	//mPlayer->replaceState(newState);
}


void PlayerState::addState(actionState newState)
{
	mPlayer->addState(newState);
}

void PlayerState::popState()
{
	return mPlayer->popState();
}