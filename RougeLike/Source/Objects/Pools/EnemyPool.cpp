#include "pch.h"
#include "EnemyPool.h"

#include "Actors/Enemies/Types/Devil.h"
#include "Actors/Enemies/Types/Orc.h"


Enemy* EnemyPool::createNewObjects(EnemyType type, int count, int& outSize) const
{
	Enemy* enemy = nullptr;

	switch (type)
	{
	case EnemyType::Devil:
	{
		enemy = new Devil[count];
		outSize = sizeof(Devil);
		break;
	}
	case EnemyType::Orc:
	{
		enemy = new Orc[count];
		outSize = sizeof(Orc);
		break;
	}
	default:
		DebugPrint(Warning, "Invalid enemy type %d, could not create %d objects\n", type, count);
		break;
	}

	return enemy;
}
