#pragma once

#include "Ability.h"


class MeleeAbility : public Ability
{
public:
	void init() override;

	bool initiate(const InputManager* input) const override;
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