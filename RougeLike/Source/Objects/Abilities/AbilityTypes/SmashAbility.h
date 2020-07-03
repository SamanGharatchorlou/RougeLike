#pragma once

#include "Ability.h"

class Texture;


class SmashAbility : public TargePositionAttackAbility
{
public:
	SmashAbility(Texture* hammerTexture, VectorF hammerSize);

	void fillValues(ValueMap& values) override;

	void activateAt(VectorF position, EffectPool* pool) override;
	void activateOn(Actor* target, EffectPool* pool) override;

	void fastUpdate(float dt) override { }
	void slowUpdate(float dt) override;
	void render() override;


private:
	bool hammerHitGround();
	void applyEffects(Actor* actor, EffectPool* effectPool);

private:
	Damage mDamage;
	float mTime;

	Texture* mHammerTexture;
	RectF mHammerRect;
	float mFallSpeed;
};