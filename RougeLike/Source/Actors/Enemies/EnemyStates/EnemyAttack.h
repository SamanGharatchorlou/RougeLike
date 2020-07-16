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
	void resume() override;

	const Type type() const { return Type::Attack; }

	bool didConnectWithTarget() const { return (bool)hitCounter; }


private:
	void addEffects();
	void updateEffects();

	void updateHasAttackedStatus();
	bool attackComplete() const;


private:
	bool mHasAttacked;

	VectorF mStartPosition;
	VectorF mAttackPosition;

	int hitCounter;
};
