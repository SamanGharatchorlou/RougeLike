#pragma once

#include "Utilities/BasicString.h"


enum class AbilityType
{
	None,
	Heal
};


inline void operator >>(AbilityType a, BasicString& str)
{
	if (a == AbilityType::Heal)
		str = "Heal";
}

inline void operator <<(AbilityType& a, const BasicString& str)
{
	if (str == "Heal")
		a = AbilityType::Heal;
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