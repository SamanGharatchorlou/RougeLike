#pragma once

#include "Objects/Effects/Effect.h"

class Ability
{
public:
	Ability(Effect* effect) : mEffect(effect) { }

	virtual std::string abilityName() const = 0;

private:
	Effect* mEffect;
};



class SlowAbility : public Ability
{
public:
	SlowAbility(Effect* effect) : Ability(effect) { }

	std::string abilityName() const { return "Slow"; }
};
