#pragma once

class Ability;
class Actor;


class AbilityBuilder
{
public:
	AbilityBuilder() : mCaster(nullptr) { }

	void init(Actor* caster) { mCaster = caster; }

	Ability* build(const BasicString& id) const;


private:
	Ability* createNewAbility(const BasicString& id) const;
	void setRangedValues(Ability* ability) const;


private:
	Actor* mCaster;
};