#pragma once

#include "PlayerState.h"


class IdleState : public PlayerState
{
public:
	IdleState(GameData* gameData, Player* player);

	void init() { }
	void handleInput();
	void update(float dt) { }
	void render() { }
	void exit() { }

	void pause() { };
	void resume() { };
};
