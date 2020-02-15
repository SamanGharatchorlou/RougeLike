#pragma once

#include "States/State.h"

class Enemy;

// TODO: I think this uses the top left as the enemy position when attacking
// if the enemy attacks the player from the left or right its position differs
class EnemyAttack : public State
{
public:
	EnemyAttack(Enemy* enemy);

	void init() override;
	void handleInput() override { }
	void slowUpdate(float dt) override;
	void fastUpdate(float dt) override;
	void render() override { }
	void exit() override;


private:
	bool beginMovement() const;
	bool returnMovement() const;
	bool endAttack() const;

private:
	Enemy* mEnemy;

	Timer<float> timer;

	float mAttackDistance;
	bool mHasAttacked;
	bool mIsAttacking;
};
