#pragma once

#include "Effect.h"

#include "Animations/Animator.h"

class DamageCollider;
class Enemy;

class StunEffect : public Effect
{
public:
	StunEffect(Animator animator) : mAnimator(animator), mMaxSize(0.0f), mTime(0.0f) { };

	void set(float time) { mTime = time; }

	void fillData(const EffectPropertyBag* properties) override;
	void clearData() override;

	void init() override;
	void fastUpdate(float dt) override { };
	void slowUpdate(float dt) override;
	void render() override;
	
	void exit() override { };

	EffectType type() const override { return EffectType::Stun; }


private:
	Animator mAnimator;
	float mMaxSize;
	float mTime;
};

