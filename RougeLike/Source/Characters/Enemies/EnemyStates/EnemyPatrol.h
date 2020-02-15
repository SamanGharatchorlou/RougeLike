#pragma once

#include "States/State.h"

class Enemy;

class EnemyPatrol : public State
{
public:
	EnemyPatrol(Enemy* enemy);

	void init() override;
	void handleInput() override { }
	void slowUpdate(float) override;
	void fastUpdate(float dt) override;
	void render() override { }
	void exit() override { }


private:
	void setPatrolPoint();
	bool hasReachedPatrolTarget() const;
	bool canSeeTarget() const;

private:
	Enemy* mEnemy;

	VectorF mPatrolTarget;
};
