#pragma once

#include "SpawnDataBuilder.h"
#include "EnemyFactory.h"

class Map;
class EnemyManager;

class AISpawnController
{
public:
	AISpawnController();
	~AISpawnController();

	void init();

	void initSpawningEnemies(const AIPathMap* map);
	void returnEnemy(Enemy* enemy);

	void spawnUnspawnedEnemies(EnemyManager* enemyManager);

	void loadSpawnPool();
	void clear();


private:
	void setRandomPosition(Formation& formation, const AIPathMap* map);
	Queue<SpawnData> spawnRandomQuad(EnemyType type, EnemyState::Type state, const AIPathMap* map);

	int getPatrollerSpacing(int mapLevel) const;


private:
	SpawnDataBuilder mSpawnData;
	EnemyFactory mFactory;

	SpawnDataList mSpawnList;

#if LIMIT_ENEMY_SPAWNS
	int current_spawn_count;
#endif
};