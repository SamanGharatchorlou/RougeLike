#pragma once

#include "Effect.h"

class DamageCollider;
class Animator;
class Enemy;

class StunEffect : public Effect
{
public:
	StunEffect(Animator* animator, VectorF size) : mAnimator(animator), mSize(size), mMaxVelocity(0.0f) { };

	void init() override;
	void fastUpdate(float dt) override { };
	void slowUpdate(float dt) override;
	void render() override;
	
	void exit() override { };


private:
	Enemy* mEnemy;
	Animator* mAnimator;
	VectorF mSize;

	float mMaxVelocity;
};

