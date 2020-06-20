#pragma once

#include "Ability.h"
#include "Objects/Attributes/Armor.h"

class ArmorAbility : public Ability
{
public:
	ArmorAbility() { };

	void fillValues(ValueMap& values) override;

	void activate(Actor* target, EffectPool* effectPool) override;
	void slowUpdate(float dt) override;

	const TargetType targetType() const override { return TargetType::Self; }


private:
	Armor mArmor;
};