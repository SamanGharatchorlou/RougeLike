#include "pch.h"
#include "EnemyPool.h"

#include "Actors/Enemies/Types/Devil.h"
#include "Actors/Enemies/Types/Orc.h"


Enemy* EnemyPool::createNewObject(EnemyType type) const
{
	Enemy* enemy = nullptr;

	switch (type)
	{
	case EnemyType::Devil:
	{
		enemy = new Devil;
		break;
	}
	case EnemyType::Orc:
	{
		enemy = new Orc;
		break;
	}
	default:
		DebugPrint(Warning, "Invalid enemy type %d\n", type);
		break;
	}

	return enemy;
}
