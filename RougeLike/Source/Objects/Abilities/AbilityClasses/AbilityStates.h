#pragma once

#include "Utilities/BasicString.h"


enum class AbilityType
{
	None,
	BasicAttack,
	Heal,
	Armor,
	Blink,
	Spikes,
	Smash,
	Charge,
	Slash,
	Count
};


template<class T>
inline AbilityType operator +(AbilityType a, T b)
{
	int sum = static_cast<int>(a) + static_cast<int>(b);
	return static_cast<AbilityType>(sum);
}


inline void operator >>(AbilityType a, BasicString& str)
{
	if (a == AbilityType::BasicAttack)
		str = "BasicAttack";
	else if (a == AbilityType::Heal)
		str = "Heal";
	else if (a == AbilityType::Blink)
		str = "Blink";
	else if (a == AbilityType::Armor)
		str = "Armor";
	else if (a == AbilityType::Spikes)
		str = "Spikes";
	else if (a == AbilityType::Smash)
		str = "Smash";
	else if (a == AbilityType::Charge)
		str = "Charge";
	else if (a == AbilityType::Slash)
		str = "Slash";
	else
		DebugPrint(Warning, "No ability to string conversion defined for ability type %d\n", a);
}

inline void operator <<(AbilityType& a, const BasicString& str)
{
	if (str == "BasicAttack")
		a = AbilityType::BasicAttack;
	else if (str == "Heal")
		a = AbilityType::Heal;
	else if (str == "Blink")
		a = AbilityType::Blink;	
	else if (str == "Armor")
		a = AbilityType::Armor;
	else if (str == "Spikes")
		a = AbilityType::Spikes;
	else if (str == "Smash")
		a = AbilityType::Smash;
	else if (str == "Charge")
		a = AbilityType::Charge;
	else if (str == "Slash")
		a = AbilityType::Slash;
	else
		DebugPrint(Warning, "No string to ability conversion defined for string '%s'\n", str);
}


enum class AbilityTarget
{
	Self,
	Melee,
	Ranged
};



enum class AbilityState
{
	None,
	Idle,
	Selected,
	Activate,
	Running,
	Cooldown,
	Finished
};