#pragma once

#include "Effect.h"

#include "Animations/Animator.h"

class DamageCollider;
class Enemy;

class StunEffect : public Effect
{
public:
	StunEffect(Animator animator, float maxSize) : mAnimator(animator), mMaxSize(maxSize) { };

	void init() override;
	void fastUpdate(float dt) override { };
	void slowUpdate(float dt) override;
	void render() override;
	
	void exit() override { };


private:
	Animator mAnimator;
	float mMaxSize;
};

