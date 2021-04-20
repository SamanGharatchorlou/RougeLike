#pragma once

#include "PlayerState.h"

class Weapon;
class Player;

class PlayerAttackState : public PlayerState
{
public:
	PlayerAttackState(Player* player);

	void init() override;
	void handleInput() override { }
	void fastUpdate(float dt) override;
	void slowUpdate(float dt) override;
	void render() override;
	void exit() override;

	bool finished() override;

private:
	Weapon* mWeapon;
};