#pragma once

#include "EnemyState.h"

class EnemyHit : public EnemyState
{
public:
	EnemyHit(Enemy* enemy);

	void init() override;
	void handleInput() override { }
	void fastUpdate(float dt) override { };
	void slowUpdate(float dt) override;
	void render() override;
	void exit() override;

	const Type type() const { return Type::Hit; }

private:
	// TODO: add 1/x^2 decay
	Timer<float> decayTimer;
	VectorF decayVelocity;

	VectorF attackTargetPosition;
};
