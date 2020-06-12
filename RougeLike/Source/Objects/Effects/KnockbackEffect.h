#pragma once

#include "Effect.h"

class DamageCollider;

class KnockbackEffect : public Effect 
{
public:
	KnockbackEffect(const DamageCollider* sourceCollider);
	KnockbackEffect(VectorF source, float force);
	 virtual ~KnockbackEffect() { }

	virtual void init() override;
	virtual void fastUpdate(float dt) override;
	virtual void slowUpdate(float dt) override;
	void render() override { };
	void exit() override { };


protected:
	virtual bool canMove(VectorF velocity, float dt) const;
	void forceDecay();


protected:
	VectorF mSource;
	float mForce;

	Timer<float> timer;
};