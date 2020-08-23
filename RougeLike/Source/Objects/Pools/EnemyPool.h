#pragma once

#include "Utilities/ObjectPool.h"
#include "Actors/Enemies/Enemy.h"

enum class EnemyType;


class EnemyPool : public ObjectPool<Enemy, EnemyType>
{
private:
	Enemy* createNewObject(EnemyType type) const override;
};