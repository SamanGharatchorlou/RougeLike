#pragma once

#include "Utilities/ObjectPool.h"
#include "Actors/Enemies/Enemy.h"

enum class EnemyType;


class EnemyPool : public ObjectPool<Enemy, EnemyType>
{
private:
	Enemy* createNewObjects(EnemyType type, int count, int& outSize) const override;
};