#pragma once


#include "Objects/Abilities/AbilityClasses/TargetAbilities.h"

class Texture;


class SmashAbility : public TargePositionAttackAbility
{
public:
	SmashAbility(Texture* hammerTexture);

	void activateAt(VectorF position, EffectPool* pool) override;
	void activateOn(Actor* target, EffectPool* pool) override;

	void enter();
	void fastUpdate(float dt) override { }
	void slowUpdate(float dt) override;
	void render() override;

	AbilityType type() const override { return AbilityType::Smash; }


private:
	bool hammerHitGround();
	void applyEffects(Actor* actor, EffectPool* effectPool);

private:
	float mTime;

	Texture* mHammerTexture;
	RectF mHammerRect;
};