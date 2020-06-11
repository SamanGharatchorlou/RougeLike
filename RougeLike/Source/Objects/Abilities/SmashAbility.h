#pragma once

#include "Ability.h"

class Texture;


class SmashAbility : public AreaAbility
{
public:
	SmashAbility(Texture* hammerTexture, VectorF hammerSize, Animator stun);

	void fillValues(ValueMap& values) override;

	void activate(VectorF position) override;
	void activate(Actor* actor) override;
	void slowUpdate(float dt) override;
	void render() override;
	void exit() override;

	const TargetType targetType() const override { return TargetType::Area_Attack; }


private:
	bool hammerHitGround();


private:
	bool requestedActivate;

	Damage mDamage;
	bool mAppliedDamage;

	Texture* mHammerTexture;
	RectF mHammerRect;
	float mFallSpeed;

	Animator mStunAnimator;
	VectorF mStunSize;
};