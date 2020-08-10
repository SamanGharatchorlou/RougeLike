#pragma once

#include "EnemySpawner.h"
#include "Actors/Enemies/EnemyPool.h"

class TextureManager;
class EffectPool;
class Environment;
class AIPathMap;

class EnemyBuilder
{
public:
	EnemyBuilder(TextureManager* textures) : mTextures(textures) { }

	void loadSpawnPool();

	std::vector<Enemy*> buildEnemies(const std::vector<SpawnData>& data, const AIPathMap* aiPathMap);
	Enemy* buildEnemy(const SpawnData& data, const XMLNode enemyNode, const AIPathMap* aiPathMap);
	void returnEnemy(Enemy* enemy);

private:
	Enemy* getBlankEnemy(EnemyType type);
	void fillActorData(Enemy* enemy, const XMLNode node) const;
	void fillSpawnData(Enemy* enemy, const SpawnData& data, const AIPathMap* aiPathMap) const;

	void setupParserMap(std::unordered_map<EnemyType, XMLParser>& parserMap, const std::vector<SpawnData>& dataList) const;

private:
	TextureManager* mTextures;
	EnemyPool mPool;
};