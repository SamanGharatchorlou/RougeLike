#pragma once

#include "States/State.h"


class Enemy;

class EnemyHit : public State
{
public:
	EnemyHit(Enemy* enemy);

	void init() override;
	void handleInput() override { }
	void slowUpdate(float dt) override;
	void fastUpdate(float dt) override;
	void render() override { }
	void exit() override;

private:
	Enemy* mEnemy;

	Timer<float> timer;
};