#pragma once

class Ability;
class Actor;

class AbilityManager
{
public:
	AbilityManager() : mActiveAbility(nullptr) { }

	void add(std::string name, Ability* ability);
	void select(const std::string& ability);

	void activate(Actor* target);

private:

	std::unordered_map<std::string, Ability*> mAbilities;
	Ability* mActiveAbility;
};