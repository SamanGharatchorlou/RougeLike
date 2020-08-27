#pragma once

#include "SpawnDataBuilder.h"
#include "Objects/Pools/EnemyPool.h"
#include "Objects/Pools/EnemyStatePool.h"

class AIPathMap;


class EnemyFactory
{
public:
	~EnemyFactory();

	void loadPools(int poolSize);
	void clear();

	std::vector<Enemy*> buildEnemies(const std::vector<SpawnData>& data, const AIPathMap* aiPathMap);
	void returnEnemy(Enemy* enemy);


private:
	Enemy* buildEnemy(const SpawnData& data, const XMLNode enemyNode, const AIPathMap* aiPathMap);
	void setupParserMap(std::unordered_map<EnemyType, XMLParser>& parserMap, const std::vector<SpawnData>& dataList) const;


private:
	EnemyPool mPool;
	EnemyStatePool mStatePool;
};