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
	void fillActorData(Enemy* enemy, const XMLNode node, EffectPool* effects);
	void fillSpawnData(Enemy* enemy, const SpawnData& data, const AIPathMap* aiPathMap);


private:
	TextureManager* mTextures;
	EnemyPool mPool;
};