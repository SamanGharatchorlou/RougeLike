#include "pch.h"
#include "EnemyPool.h"

#include "Actors/Enemies/Types/Devil.h"


void EnemyPool::load()
{
	std::vector<BasicString> enemyNames = FileManager::Get()->fileNamesInFolder(FileManager::Config_Enemies);
	EnemyType type = EnemyType::None;

	for (int i = 0; i < enemyNames.size(); i++)
	{
		type << enemyNames[i];
		addNewObjects(type, 50);
	}
}


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