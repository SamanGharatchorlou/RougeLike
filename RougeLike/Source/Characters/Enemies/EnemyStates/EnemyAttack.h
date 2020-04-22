#pragma once

#include "EnemyState.h"

class Enemy;

// TODO: I think this uses the top left as the enemy position when attacking
// if the enemy attacks the player from the left or right its position differs
class EnemyAttack : public EnemyState
{
public:
	EnemyAttack(Enemy* enemy);

	void init() override;
	void handleInput() override { }
	void slowUpdate(float dt) override;
	void fastUpdate(float dt) override;
	void render() override;
	void exit() override;

	const Type type() const { return Type::Attack; }


private:
	bool forwardAttackComplete();
	bool attackComplete() const;

private:
	bool mHasAttacked;

	VectorF startingPosition;
	VectorF attackTargetPosition;

	int hitCounter;
};
