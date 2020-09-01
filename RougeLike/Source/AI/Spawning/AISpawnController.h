#pragma once

#include "SpawnDataBuilder.h"
#include "EnemyFactory.h"

class Map;

class AISpawnController
{
public:
	~AISpawnController();

	std::vector<Enemy*> getNewLevelSpawns(const AIPathMap* map);
	void returnEnemy(Enemy* enemy);

	void loadSpawnPool(int poolSize);
	void clear();

private:
	void setRandomPosition(Formation& formation, const AIPathMap* map);
	SpawnDataList spawnRandomQuad(EnemyType type, EnemyState::Type state, const AIPathMap* map);

private:
	SpawnDataBuilder mSpawnData;
	EnemyFactory mFactory;
};