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

	Enemy* buildEnemy(const SpawnData& data, Environment* environment, const AIPathMap* aiPathMap);
	void returnEnemy(Enemy* enemy);

private:
	Enemy* getBlankEnemy(const SpawnData& data);
	void fillStaticTypeData(Enemy* enemy, const XMLParser& parser, EffectPool* effects);
	void fillEnvironmentData(Enemy* enemy, Environment* environment, const AIPathMap* aiPathMap);
	void fillSpawnData(Enemy* enemy, const SpawnData& data);


private:
	TextureManager* mTextures;
	EnemyPool mPool;
};