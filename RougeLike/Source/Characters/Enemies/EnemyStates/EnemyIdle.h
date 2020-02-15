#pragma once

#include "States/State.h"

class Enemy;

class EnemyIdle : public State
{
public:
	EnemyIdle(Enemy* enemy) : mEnemy(enemy), idleTime(-1) { }

	void init() override;
	void handleInput() override { }
	void slowUpdate(float) override;
	void fastUpdate(float dt) override;
	void render() override { }
	void exit() override { }

	void setIdleTime(float time) { idleTime = time; }

private:
	bool canSeeTarget() const;

private:
	Enemy* mEnemy;

	Timer<float> timer;
	float idleTime;
};

