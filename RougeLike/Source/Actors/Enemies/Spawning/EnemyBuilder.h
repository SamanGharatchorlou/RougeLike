#pragma once

#include "EnemySpawner.h"
#include "Objects/Pools/EnemyPool.h"
#include "Objects/Pools/EnemyStatePool.h"

class EffectPool;
class Environment;
class AIPathMap;

class EnemyBuilder
{
public:
	~EnemyBuilder();

	void loadSpawnPool(int poolSize);
	void clear();

	std::vector<Enemy*> buildEnemies(const std::vector<SpawnData>& data, const AIPathMap* aiPathMap);
	Enemy* buildEnemy(const SpawnData& data, const XMLNode enemyNode, const AIPathMap* aiPathMap);
	void returnEnemy(Enemy* enemy);

private:
	Enemy* getBlankEnemy(EnemyType type);
	void fillActorData(Enemy* enemy, const XMLNode node);
	void fillSpawnData(Enemy* enemy, const SpawnData& data, const AIPathMap* aiPathMap) const;

	void setupParserMap(std::unordered_map<EnemyType, XMLParser>& parserMap, const std::vector<SpawnData>& dataList) const;

private:
	EnemyPool mPool;
	EnemyStatePool mStatePool;
};