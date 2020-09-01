#pragma once

#include "Ability.h"
#include "Utilities/Quad2D.h"


class MeleeAbility : public Ability
{
public:
	virtual void activate(VectorF direction) = 0;
	bool activateOn(Actor* target, EffectPool* effectPool) override;

	virtual AbilityTarget targetType() const override { return AbilityTarget::Melee; }

	virtual void baseExit() override;

private:
	virtual void applyEffects(Actor* actor, EffectPool* effectPool) = 0;


protected:
	VectorF mTargetDirection;

	Quad2D<float> mQuad;
	std::unordered_set<Actor*> mHitList;
};