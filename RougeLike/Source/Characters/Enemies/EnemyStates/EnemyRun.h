#pragma once

#include "States/State.h"

#include "AI/AIPathing.h"

class Enemy;

class EnemyRun : public State
{
public:
	EnemyRun(Enemy* enemy);

	void init() override;
	void handleInput() override { }
	void slowUpdate(float) override;
	void fastUpdate(float dt) override;
	void render() override { }
	void exit() override { }

	void pause() override { };
	void resume() override;

private:
	void updatePath();
	void updateMovement(VectorF target);

	bool inChaseRange() const;
	bool inAttackRange() const;

private:
	Enemy* mEnemy;

	AIPathing mAIPathing;
	Path mPath;
};
