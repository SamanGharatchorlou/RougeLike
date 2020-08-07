#pragma once

#include "Objects/Abilities/AbilityClasses/TargetAbilities.h"
#include "Objects/Properties/PropertyTypes/Armor.h"

class ArmorAbility : public TargetSelfAbility
{
public:
	ArmorAbility() { };

	//void fillValues(ValueMap& values) override;

	void activate(EffectPool* pool) override;

	void fastUpdate(float dt) override { }
	void slowUpdate(float dt) override;

	AbilityTarget targetType() const override { return AbilityTarget::Self; }


protected:
	void applyEffects(EffectPool* pool);
	void updateUI();


private:
	Armor mArmor;
};