#pragma once

class Ability;
class Actor;


class AbilityBuilder
{
public:
	AbilityBuilder(Actor* caster) : mCaster(caster) { }

	Ability* build(const BasicString& id) const;


private:
	Ability* createNewAbility(const BasicString& id) const;
	void setRangedValues(Ability* ability) const;

private:
	Actor* mCaster;
};