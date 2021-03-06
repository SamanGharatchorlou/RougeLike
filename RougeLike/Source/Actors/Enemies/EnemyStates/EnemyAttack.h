#pragma once

#include "EnemyState.h"

class EnemyAttack : public EnemyState
{
public:
	EnemyAttack() : mHasAttacked(false), mDidHit(false) { }

	void init() override;
	void handleInput() override { }
	void slowUpdate(float dt) override;
	void fastUpdate(float dt) override;
	void render() override;
	void exit() override;
	void resume() override;

	const Type type() const { return Type::Attack; }

	bool didConnectWithTarget() const { return mDidHit; }


private:
	void initProperties();
	void updateHasAttackedStatus();
	bool attackComplete() const;


private:
	bool mHasAttacked;
	bool mDidHit;

	VectorF mStartPosition;
	VectorF mEndPosition;
	VectorF mDirection;
};
