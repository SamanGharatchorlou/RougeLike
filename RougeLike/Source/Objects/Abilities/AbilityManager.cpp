#include "pch.h"
#include "AbilityManager.h"

#include "Ability.h"


void AbilityManager::add(std::string name, Ability* ability)
{
	mAbilities[name] = ability;
}

void AbilityManager::select(const std::string& ability)
{
	auto search = mAbilities.find(ability);

	if (search != mAbilities.end())
	{
		mActiveAbility = search->second;
	}
	else
	{
		DebugPrint(Warning, "Player does not have the '%s' ability\n", ability.c_str());
		mActiveAbility = nullptr;
	}
}

void AbilityManager::activate(Actor* target)
{
	mActiveAbility->activate(target);
}