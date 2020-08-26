#pragma once

#include "State.h"

struct GameData;


class PauseState : public State
{
public:
	PauseState(GameData* gameData);

	void init() override;
	void slowUpdate(float dt) override;
	void fastUpdate(float dt) override { };
	void handleInput() override;
	void render() override;
	void exit() override { }


private:
	GameData* mGameData;
};