#pragma once


enum class EnemyType
{
	None,
	Imp,
	Angel,
	Devil
};



inline void operator >>(EnemyType a, std::string& str)
{
	if (a == EnemyType::Devil)
		str = "Devil";
}

EnemyType stringToEnemyType(const std::string& type);