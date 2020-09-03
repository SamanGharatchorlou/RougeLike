#pragma once

#include "PlayerState.h"

class InputManager;
class Player;


class PlayerDeadState : public PlayerState
{
public:
	PlayerDeadState(Player* player);

	void init() override;
	void handleInput() override { }
	void handleInput(const InputManager* input) { } // TODO do i need this?
	void fastUpdate(float dt) override;
	void slowUpdate(float dt) override;
	void render() override { };
	void exit() override;

	bool finished() override;

private:
	Player* mPlayer;
	TimerF mDeathTimer;
};