#pragma once

#include "States/State.h"

class Player;
class MeleeWeapon;


class MeleeAttackState : public State
{
public:
	MeleeAttackState(Player* player);

	void init() { }
	void handleInput() { };
	void slowUpdate(float dt) { }
	void fastUpdate(float dt);
	void render() { };
	void exit();

	void pause() { };
	void resume() { };

private:
	Player* mPlayer;

	MeleeWeapon* mWeapon;
	double mRotationSum;
};
