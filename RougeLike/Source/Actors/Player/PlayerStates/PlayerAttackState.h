#pragma once

#include "PlayerState.h"

class InputManager;
class MeleeWeapon;
class Player;


class PlayerAttackState : public PlayerState
{
public:
	PlayerAttackState(Player* player);

	void init() override;
	void handleInput() override { }
	void handleInput(const InputManager* input) { } // TODO do i need this?
	void fastUpdate(float dt) override;
	void slowUpdate(float dt) override;
	void render() override;
	void exit() override;

	bool finished() override;

private:
	MeleeWeapon* mWeapon;
};