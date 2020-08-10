#include "pch.h"
#include "EnemyBuilder.h"

#include "Map/Environment.h"
#include "Actors/Enemies/Enemy.h"
#include "Actors/Enemies/EnemyPool.h"


void EnemyBuilder::loadSpawnPool()
{
	mPool.load();
}


void EnemyBuilder::returnEnemy(Enemy* enemy)
{
	mPool.returnObject(enemy);
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
	enemy->setCharacter(node, mTextures);
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