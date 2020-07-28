#include "pch.h"
#include "EnemyPool.h"

#include "Actors/Enemies/Types/Devil.h"


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
	default:
		DebugPrint(Warning, "Invalid enemy type %d\n", type);
		break;
	}

	return enemy;
}


void EnemyPool::returnObject(Enemy* enemy)
{
	mPool[enemy->type()].push(enemy);
}