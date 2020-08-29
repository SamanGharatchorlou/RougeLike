#pragma once

class Ability;
class Actor;
enum class AbilityType;

class AbilityBuilder
{
public:
	Ability* build(AbilityType type, Actor* caster) const;

private:
	Ability* createNewAbility(AbilityType type) const;
};