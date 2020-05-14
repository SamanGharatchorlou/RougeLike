#pragma once

#include "EnemyState.h"

class Enemy;


class EnemyAttack : public EnemyState
{
public:
	EnemyAttack(Enemy* enemy);

	void init() override;
	void handleInput() override { }
	void slowUpdate(float dt) override;
	void fastUpdate(float dt) override;
	void render() override;
	void exit() override { };

	const Type type() const { return Type::Attack; }

	bool didConnectWithTarget() const { return (bool)hitCounter; }


private:
	void updateHasAttackedStatus();
	bool attackComplete() const;

private:
	bool mHasAttacked;

	VectorF startingPosition;
	VectorF attackTargetPosition;

	int hitCounter;
};
