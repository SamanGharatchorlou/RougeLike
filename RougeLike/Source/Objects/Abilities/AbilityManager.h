#pragma once

#include "Ability.h"

class AbilityManager
{
public:
	void add(Ability* ability);

private:
	std::vector<Ability*> mAbilities;

};