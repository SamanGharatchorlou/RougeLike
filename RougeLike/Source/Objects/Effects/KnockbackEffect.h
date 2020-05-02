#pragma once

#include "Effect.h"
#include "Objects/Attributes/Physics.h"


class KnockbackEffect : public Effect 
{
public:
	KnockbackEffect(Physics* physics, VectorF source, float force);
	~KnockbackEffect() { }

	void init() override;
	void fastUpdate(float dt) override;
	void slowUpdate(float dt) override;
	void render() override;
	void exit() override;

private:
	bool canMove(VectorF velocity, float dt);

private:
	Physics* mPhysics;
	VectorF mSource;
	float mForce;

	Timer<float> timer;
};