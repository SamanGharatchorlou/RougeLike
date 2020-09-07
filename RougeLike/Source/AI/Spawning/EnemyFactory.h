#pragma once

#include "SpawnDataBuilder.h"
#include "Objects/Pools/EnemyPool.h"
#include "Objects/Pools/EnemyStatePool.h"

class AIPathMap;
class Ability;


class EnemyFactory
{
public:
	~EnemyFactory();

	void loadPools();
	void clear();

	std::vector<Enemy*> buildEnemies(const std::vector<SpawnData>& data, const AIPathMap* aiPathMap);
	void returnEnemy(Enemy* enemy);


private:
	Enemy* buildEnemy(const SpawnData& data, const XMLNode enemyNode, const AIPathMap* aiPathMap);
	Enemy* buildEnemy(const SpawnData& data, const XMLNode baseNode, const XMLNode animationNode, const AIPathMap* aiPathMap);
	void setupParserMap(std::unordered_map<BasicString, XMLParser>& parserMap, const std::vector<SpawnData>& dataList) const;

	Ability* buildBasicAttack(Enemy* enemy, const XMLNode enemyNode);

private:
	EnemyPool mPool;
	EnemyStatePool mStatePool;
};