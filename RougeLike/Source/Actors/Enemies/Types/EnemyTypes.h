#pragma once


enum class EnemyType
{
	None,
	Imp,
	Angel,
	Devil
};

EnemyType stringToEnemyType(const std::string& type);