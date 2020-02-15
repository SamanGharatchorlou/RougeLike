#pragma once

#include "PlayerState.h"

class RunState : public PlayerState
{
public:
	RunState(GameData* gameData, Player* player);

	void init() { }
	void handleInput();
	void update(float dt);
	void render() { };
	void exit() { };

	void pause() { };
	void resume() { };
};
