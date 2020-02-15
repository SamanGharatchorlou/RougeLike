#pragma once

#include "States/State.h"

class Enemy;

class EnemyAlert : public State
{
public:
	EnemyAlert(Enemy* enemy) : mEnemy(enemy), mHasFlipped(false) { }

	void init() override;
	void handleInput() override { }
	void slowUpdate(float) override;
	void fastUpdate(float dt) override;
	void render() override { }
	void exit() override { }

private:
	Enemy* mEnemy;

	Timer<float> timer;
	bool mHasFlipped;
};