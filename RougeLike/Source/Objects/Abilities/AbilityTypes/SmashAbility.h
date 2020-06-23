#pragma once

#include "Ability.h"

class Texture;


class SmashAbility : public AreaAbility
{
public:
	SmashAbility(Texture* hammerTexture, VectorF hammerSize);

	void fillValues(ValueMap& values) override;

	void activate(VectorF position) override;
	void activate(Actor* actor, EffectPool* effectPool) override;
	void fastUpdate(float dt) override { }
	void slowUpdate(float dt) override;
	void render() override;
	void exit() override;

	const TargetType targetType() const override { return TargetType::Area_Attack; }


private:
	bool hammerHitGround();


private:
	bool requestedActivate;

	Damage mDamage;
	float mTime;

	bool mAppliedDamage;

	Texture* mHammerTexture;
	RectF mHammerRect;
	float mFallSpeed;
};