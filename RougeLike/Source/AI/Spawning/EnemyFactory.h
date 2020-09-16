#pragma once

#include "SpawnDataBuilder.h"
#include "Objects/Pools/EnemyPool.h"
#include "Objects/Pools/EnemyStatePool.h"

class AIPathMap;
class Ability;


class EnemyFactory
{
public:
	EnemyFactory() { };
	~EnemyFactory();

	void initSpawnInfo();
	void loadPools();
	void clear();

	Enemy* buildEnemy(const SpawnData& dataList, const AIPathMap* aiPathMap);
	void returnEnemy(Enemy* enemy);

	void topUpPool(const SpawnDataList& data);

private:
	Enemy* setEnemyData(Enemy* enemy, const SpawnData& data, const XMLNode baseNode, const XMLNode animationNode, const AIPathMap* aiPathMap);

	Ability* buildBasicAttack(Enemy* enemy, const XMLNode enemyNode);


private:
	EnemyPool mPool;
	EnemyStatePool mStatePool;

	std::unordered_map<BasicString, XMLParser> mSpawnInfo;
};
