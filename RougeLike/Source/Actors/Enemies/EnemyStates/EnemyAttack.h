#pragma once

#include "EnemyState.h"

class Enemy;


class EnemyAttack : public EnemyState
{
public:
	EnemyAttack();

	void enter() override;
	void init() override;
	void handleInput() override { }
	void slowUpdate(float dt) override;
	void fastUpdate(float dt) override;
	void render() override;
	void exit() override;
	void resume() override;

	const Type type() const { return Type::Attack; }

	bool didConnectWithTarget() const { return (bool)mHitCounter; }


private:
	void updateHasAttackedStatus();
	bool attackComplete() const;


private:
	bool mHasAttacked;

	VectorF mStartPosition;
	VectorF mAttackPosition;

	int mHitCounter;
};
