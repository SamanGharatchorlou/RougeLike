#pragma once

#include "States/State.h"

class Enemy;

class EnemyDead : public State
{
public:
	EnemyDead(Enemy* enemy) : mEnemy(enemy) { }

	void init() override;
	void handleInput() override { }
	void slowUpdate(float dt) override;
	void fastUpdate(float dt) override { };
	void render() override { }
	void exit() override { };

private:
	Enemy* mEnemy;

};
