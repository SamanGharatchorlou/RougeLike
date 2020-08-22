#pragma once

#include "Objects/Abilities/AbilityClasses/TargetAbilities.h"


class SlashAbility : public TargePositionAttackAbility
{
public:
	SlashAbility() : mSlashCount(0) { mOnlyDirectional = true; }

	void activateAt(VectorF position, EffectPool* effectPool) override;
	void activateOn(Actor* actor, EffectPool* effectPool) override;

	void fastUpdate(float dt) override { }
	void slowUpdate(float dt) override;
	//void render();
	void exit() override;

	AbilityType type() const override { return AbilityType::Slash; }


private:
	void slashOnce();
	void applyEffects(Actor* actor, EffectPool* effectPool);

private:
	int mSlashCount;
};