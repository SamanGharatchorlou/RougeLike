#pragma once

#include "Utilities/ObjectPool.h"
#include "Types/EnemyTypes.h"

class Enemy;

class EnemyPool : public ObjectPool<Enemy, EnemyType>
{
public:
	// add load function (info pulled from config file?)
	void load() override { }
	void returnObject(Enemy* enemy) override;

private:

	Enemy* createNewObject(EnemyType type) const override;

};