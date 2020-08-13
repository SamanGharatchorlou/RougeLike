#pragma once

#include "Utilities/BasicString.h"


enum class AbilityType
{
	None,
	Heal,
	Blink,
	Spikes,
	Smash,
};


inline void operator >>(AbilityType a, BasicString& str)
{
	if (a == AbilityType::Heal)
		str = "Heal";
	else if (a == AbilityType::Blink)
		str = "Blink";
	else if (a == AbilityType::Spikes)
		str = "Spikes";
	else if (a == AbilityType::Smash)
		str = "Smash";
	else
		DebugPrint(Warning, "No ability to string conversion defined for ability type %d\n", a);
}

inline void operator <<(AbilityType& a, const BasicString& str)
{
	if (str == "Heal")
		a = AbilityType::Heal;
	else if (str == "Blink")
		a = AbilityType::Blink;
	else if (str == "Spikes")
		a = AbilityType::Spikes;
	else if (str == "Smash")
		a = AbilityType::Smash;
	else
		DebugPrint(Warning, "No string to ability conversion defined for string '%s'\n", str);
}


enum class AbilityTarget
{
	Self,
	Actor,
	Position,
	AttackArea,
};


enum class AbilityState
{
	None,
	Idle,
	Selected,
	Activate,
	Running,
	Finished
};