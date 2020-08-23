#pragma once

class Ability;
class Actor;
enum class AbilityType;


class AbilityBuilder
{
public:
	AbilityBuilder() : mCaster(nullptr) { }

	void init(Actor* caster) { mCaster = caster; }

	Ability* build(AbilityType type) const;


private:
	Ability* createNewAbility(AbilityType type) const;
	void setRangedValues(Ability* ability) const;


private:
	Actor* mCaster;
};