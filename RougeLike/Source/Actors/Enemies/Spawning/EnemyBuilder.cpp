#include "pch.h"
#include "EnemyBuilder.h"

#include "Game/Environment.h"
#include "Actors/Enemies/Enemy.h"
#include "Objects/Pools/EnemyPool.h"


void EnemyBuilder::loadSpawnPool()
{
	std::vector<EnemyType> types;
	for (EnemyType type = EnemyType::None + 1; type < EnemyType::Count; type = type + 1)
	{
		types.push_back(type);
	}

	mPool.load(types, 50);
}


void EnemyBuilder::returnEnemy(Enemy* enemy)
{
	mPool.returnObject(enemy, enemy->type());
}



std::vector<Enemy*> EnemyBuilder::buildEnemies(const std::vector<SpawnData>& dataList, const AIPathMap* aiPathMap)
{
	std::unordered_map<EnemyType, XMLParser> parserMap;
	setupParserMap(parserMap, dataList);

	std::vector<Enemy*> enemies;

	for (int i = 0; i < dataList.size(); i++)
	{
		const SpawnData& data = dataList[i];
		EnemyType type = data.type;
		XMLNode node = parserMap[type].rootNode();

		Enemy* enemy = buildEnemy(data, node, aiPathMap);
		enemies.push_back(enemy);
	}

	return enemies;
}


Enemy* EnemyBuilder::buildEnemy(const SpawnData& data, const XMLNode enemyNode, const AIPathMap* aiPathMap)
{
	EnemyType type = data.type;
	BasicString enemyTypeString;
	type >> enemyTypeString;

	Enemy* enemy = getBlankEnemy(type);
	fillActorData(enemy, enemyNode);
	fillSpawnData(enemy, data, aiPathMap);
	return enemy;
}


Enemy* EnemyBuilder::getBlankEnemy(EnemyType type)
{
	return mPool.getObject(type);
}


void EnemyBuilder::fillActorData(Enemy* enemy, const XMLNode node) const
{
	enemy->setCharacter(node);
}


void EnemyBuilder::fillSpawnData(Enemy* enemy, const SpawnData& data, const AIPathMap* aiPathMap) const
{
	enemy->spawn(data.state, data.position, aiPathMap);
}


void EnemyBuilder::setupParserMap(std::unordered_map<EnemyType, XMLParser>& parserMap, const std::vector<SpawnData>& dataList) const
{
	for (int i = 0; i < dataList.size(); i++)
	{
		EnemyType type = dataList[i].type;
		BasicString enemyTypeString;
		type >> enemyTypeString;

		if (parserMap.count(type) == 0)
		{
			parserMap[type].parseXML(FileManager::Get()->findFile(FileManager::Configs_Objects, enemyTypeString));
		}
	}
}