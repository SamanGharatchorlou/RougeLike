#pragma once

#include "Ability.h"
#include "Objects/Attributes/Armor.h"

class ArmorAbility : public TargetSelfAbility
{
public:
	ArmorAbility() { };

	void fillValues(ValueMap& values) override;

	void activate(EffectPool* pool) override;

	void fastUpdate(float dt) override { }
	void slowUpdate(float dt) override;

	const TargetType targetType() const override { return TargetType::Self; }


protected:
	void applyEffects(EffectPool* pool);
	void updateUI();


private:
	Armor mArmor;
};