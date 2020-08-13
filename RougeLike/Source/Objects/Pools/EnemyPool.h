#pragma once

#include "Utilities/ObjectPool.h"

class Enemy;
enum class EnemyType;


class EnemyPool : public ObjectPool<Enemy, EnemyType>
{
private:
	Enemy* createNewObject(EnemyType type) const override;
};