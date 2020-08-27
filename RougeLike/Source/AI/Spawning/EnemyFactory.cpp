#include "pch.h"
#include "EnemyFactory.h"

#include "Actors/Enemies/Enemy.h"



EnemyFactory::~EnemyFactory()
{
	clear();
}


void EnemyFactory::loadPools(int poolSize)
{
	std::vector<EnemyType> enemyTypes;
	for (EnemyType type = EnemyType::None + 1; type < EnemyType::Count; type = type + 1)
	{
		enemyTypes.push_back(type);
	}
	mPool.load(enemyTypes, poolSize);


	std::vector<EnemyState::Type> enemyStates;
	for (EnemyState::Type state = EnemyState::Type::None + 1; state < EnemyState::Type::Count; state = state + 1)
	{
		enemyStates.push_back(state);
	}
	mStatePool.load(enemyStates, poolSize);
}

void EnemyFactory::clear()
{
	mPool.freeAll();
	mStatePool.freeAll();
}


void EnemyFactory::returnEnemy(Enemy* enemy)
{
	mPool.returnObject(enemy, enemy->type());
}



std::vector<Enemy*> EnemyFactory::buildEnemies(const std::vector<SpawnData>& dataList, const AIPathMap* aiPathMap)
{
	// Enemy config files store
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


Enemy* EnemyFactory::buildEnemy(const SpawnData& data, const XMLNode enemyNode, const AIPathMap* aiPathMap)
{
	EnemyType type = data.type;
	BasicString enemyTypeString;
	type >> enemyTypeString;

	Enemy* enemy = mPool.getObject(type);

	enemy->setCharacter(enemyNode);
	enemy->setStatePool(&mStatePool);
	enemy->spawn(data.state, data.position, aiPathMap);

	return enemy;
}


void EnemyFactory::setupParserMap(std::unordered_map<EnemyType, XMLParser>& parserMap, const std::vector<SpawnData>& dataList) const
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