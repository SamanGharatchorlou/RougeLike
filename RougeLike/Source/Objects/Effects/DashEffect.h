#pragma once

#include "Effect.h"

class DashEffect : public Effect
{
public:
	DashEffect(VectorF target, float distance) : mTarget(target), mDistance(distance) { };

	void init() override { };
	void fastUpdate(float dt) override;
	void slowUpdate(float dt) override;
	void render() override { }
	void exit() override { };


private:
	bool canMove(VectorF velocity, float dt) const;


private:
	VectorF mTarget;
	float mDistance;
};