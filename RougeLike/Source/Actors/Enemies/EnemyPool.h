#pragma once

#include "Utilities/ObjectPool.h"
#include "Types/EnemyTypes.h"

class Enemy;

class EnemyPool : public ObjectPool<Enemy*, EnemyType>
{
	enum ObjectStatus
	{
		None,
		Uninitialised,
		Available,
		Active,
		Inactive,
	};

	using EnemyObject = std::pair<Enemy*, ObjectStatus>;

private:

	Enemy* createNewObject(EnemyType type) const override { return nullptr; }

private:
	// move this functionality into a class?
	std::vector<EnemyObject> mEnemyPool;
	std::vector<Enemy*> mActiveEnemies;

	std::list<Enemy*> mActiveEnims;
};