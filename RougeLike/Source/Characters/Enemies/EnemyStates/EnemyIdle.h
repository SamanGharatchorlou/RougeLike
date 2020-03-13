#pragma once

#include "States/State.h"

class Enemy;

class EnemyIdle : public State
{
public:
	EnemyIdle(Enemy* enemy) : mEnemy(enemy) { }

	void init() override;
	void handleInput() override { }
	void fastUpdate(float dt) override { }
	void slowUpdate(float) override;
	void render() override { }
	void exit() override { }

private:
	bool canSeeTarget() const;

private:
	Enemy* mEnemy;

	Timer<float> timer;
};

