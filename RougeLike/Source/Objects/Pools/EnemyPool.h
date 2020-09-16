#pragma once

#include "Utilities/ObjectPool.h"
#include "Actors/Enemies/Enemy.h"

enum class EnemyType;


class EnemyPool : public ObjectPool<Enemy, EnemyType>
{
public:
	int size(EnemyType type);

private:
	Enemy* createNewObject(EnemyType type) const override;
};