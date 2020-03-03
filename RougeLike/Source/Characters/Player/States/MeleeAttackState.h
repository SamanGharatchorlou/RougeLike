#pragma once

#include "PlayerState.h"

struct GameData;
class MeleeWeapon;

class MeleeAttackState : public PlayerState
{
public:
	MeleeAttackState(GameData* gameData, Player* player);

	void init() { }
	void handleInput() { };
	void slowUpdate(float dt) { }
	void fastUpdate(float dt);
	void render() { };
	void exit();

	void pause() { };
	void resume() { };

private:
	MeleeWeapon* mWeapon;
	double mRotationSum;
};
