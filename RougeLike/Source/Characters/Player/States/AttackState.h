#pragma once

#include "PlayerState.h"

struct GameData;
class Weapon;

class AttackState : public PlayerState
{
public:
	AttackState(GameData* gameData, Player* player);

	void init() { }
	void handleInput() { };
	void slowUpdate(float dt) { }
	void fastUpdate(float dt);
	void render() { };
	void exit();

	void pause() { };
	void resume() { };

private:
	Weapon* mWeapon;
	double mRotationSum;
};
