#pragma once

#include "Effect.h"

class DamageCollider;

class KnockbackEffect : public Effect 
{
public:
	KnockbackEffect(const DamageCollider* sourceCollider);
	KnockbackEffect(VectorF source, float force);
	~KnockbackEffect() { }

	void init() override;
	void fastUpdate(float dt) override;
	void slowUpdate(float dt) override;
	void render() override { };
	void exit() override { };

private:
	bool canMove(VectorF velocity, float dt) const;

private:
	VectorF mSource;
	float mForce;

	Timer<float> timer;
};