#pragma once

#include "EnemyState.h"

#include "Collisions/WallCollisionTracker.h"

class EnemyCharge : public EnemyState
{
public:
	void enter() override;
	void init() override;
	void handleInput() override { }
	void fastUpdate(float dt) override;
	void slowUpdate(float dt) override;
	void render() override;
	void exit() override;
	void resume() override;

	const Type type() const { return Type::PreAttack; }


private:
	void updateHasAttackedStatus();


private:
	bool mEndAttack;
	bool mStunned;

	VectorF mDirection;
	VectorF mStartPosition;

	int mHitCounter;

	WallCollisionTracker mWallCollisions;
};