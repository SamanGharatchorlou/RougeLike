#pragma once

#include "States/State.h"

class Enemy;

class EnemyPreAttack : public State
{
public:
	EnemyPreAttack(Enemy* enemy);

	void init() override;
	void handleInput() override { }
	void slowUpdate(float dt) override;
	void fastUpdate(float dt) override;
	void render() override { }
	void exit() override { };


private:
	bool inAttackRange() const;


private:
	Enemy* mEnemy;

	Timer<float> timer;
};
