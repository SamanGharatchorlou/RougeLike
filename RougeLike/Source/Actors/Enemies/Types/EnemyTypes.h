#pragma once


enum class EnemyType
{
	None,
	Imp,
	Angel,
	Devil
};



inline void operator >>(EnemyType a, BasicString& str)
{
	if (a == EnemyType::Devil)
		str = "Devil";
}

EnemyType stringToEnemyType(const BasicString& type);