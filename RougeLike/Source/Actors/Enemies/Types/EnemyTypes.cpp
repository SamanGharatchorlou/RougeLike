#include "pch.h"
#include "EnemyTypes.h"

// TODO: replace with << operator
EnemyType stringToEnemyType(const BasicString& type)
{
	if (type == "Devil")
		return EnemyType::Devil;
	else
	{
		DebugPrint(Warning, "No enemy type string '%s' defined, no enemy type returned\n", type.c_str());
		return EnemyType::None;
	}
}