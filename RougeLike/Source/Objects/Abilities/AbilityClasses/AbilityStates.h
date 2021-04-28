#pragma once

#include "Utilities/BasicString.h"


enum class AbilityType
{
	None,

	// Basic Attack Types
	MeleeAttack,
	RangeAttack,

	// abilities
	Heal,
	Armor,
	Blink,
	Spikes,
	Smash,
	Charge,
	Slash,
	Lightning,

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
	switch (a)
	{
	case AbilityType::MeleeAttack:
		str = "MeleeAttack";
		break;
	case AbilityType::Heal:
		str = "Heal";
		break;
	case AbilityType::Blink:
		str = "Blink";
		break;
	case AbilityType::Armor:
		str = "Armor";
		break;
	case AbilityType::Spikes:
		str = "Spikes";
		break;
	case AbilityType::Smash:
		str = "Smash";
		break;
	case AbilityType::Charge:
		str = "Charge";
		break;
	case AbilityType::Slash:
		str = "Slash";
		break;
	case AbilityType::Lightning:
		str = "Lightning";
		break;
	default:
		DebugPrint(Warning, "No ability to string conversion defined for ability type %d\n", a);
		break;
	}
}

inline void operator <<(AbilityType& a, const BasicString& str)
{
	if (str == "MeleeAttack")
		a = AbilityType::MeleeAttack;
	else if (str == "RangeAttack")
		a = AbilityType::RangeAttack;
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
	else if (str == "Lightning")
		a = AbilityType::Lightning;
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