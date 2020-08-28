#pragma once

#include "Objects/Abilities/AbilityClasses/MeleeAbility.h"


class SlashAbility : public MeleeAbility
{
public:
	SlashAbility() : mSlashCount(0) { }

	void activate(VectorF position) override;

	void fastUpdate(float dt) override;
	void slowUpdate(float dt) override;
	void render() override;
	void exit() override;

	AbilityType type() const override { return AbilityType::Slash; }


private:
	void setQuadRect();
	void slashOnce();

	void applyEffects(Actor* actor, EffectPool* effectPool) override;


private:
	int mSlashCount;
	VectorF mTargetDirection;
};