#pragma once

#include "EnemyState.h"

class EnemyHit : public EnemyState
{
public:
	EnemyHit(Enemy* enemy);

	void init() override;
	void handleInput() override { }
	void slowUpdate(float dt) override;
	void fastUpdate(float dt) override;
	void render() override;
	void exit() override;

	const Type type() const { return Type::Hit; }

private:
	bool canMove(VectorF velocity, float dt);

private:
	Timer<float> decayTimer;
	VectorF decayVelocity;

	VectorF attackTargetPosition;

	float mKnockbackForce;
};
