#include "pch.h"
#include "EnemyBuilder.h"

#include "Map/Environment.h"
#include "Actors/Enemies/Enemy.h"
#include "Actors/Enemies/EnemyPool.h"
//#include "AI/AIPathMap.h"

void EnemyBuilder::loadSpawnPool()
{
	mPool.addNewObjects(EnemyType::Devil, 50);
}

// TODO: accept a list of data, no need to build a new parser each time (for the same type)
Enemy* EnemyBuilder::buildEnemy(const SpawnData& data, Environment* environment, const AIPathMap* aiPathMap)
{
	std::string enemyType;
	data.type >> enemyType;

	XMLParser parser(FileManager::Get()->findFile(FileManager::Configs_Objects, enemyType));
	Enemy* enemy = getBlankEnemy(data);

	fillStaticTypeData(enemy, parser, environment->effectPool());
	fillSpawnData(enemy, data, aiPathMap);
	enemy->set(environment);
#if !IGNORED_BY_ENEMIES
	enemy->setTarget(environment->actors()->player()->get());
#endif

	return enemy;
}


Enemy* EnemyBuilder::getBlankEnemy(const SpawnData& data)
{
	std::string enemyType;
	data.type >> enemyType;

	return mPool.getObject(data.type);
}


void EnemyBuilder::fillStaticTypeData(Enemy* enemy, const XMLParser& parser, EffectPool* effects)
{
	enemy->setCharacter(parser, mTextures);
	enemy->readEffects(parser, effects);
}


void EnemyBuilder::fillSpawnData(Enemy* enemy, const SpawnData& data, const AIPathMap* aiPathMap)
{
	enemy->spawn(data.state, data.position, aiPathMap);
}


void EnemyBuilder::returnEnemy(Enemy* enemy)
{
	mPool.returnObject(enemy);
}