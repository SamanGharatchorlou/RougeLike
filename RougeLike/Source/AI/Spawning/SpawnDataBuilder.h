#pragma once

#include "Actors/Enemies/EnemyStates/EnemyState.h"

class AIPathMap;
class Formation;
enum class EnemyType;

struct SpawnData
{
	SpawnData(EnemyType enemyType, EnemyState::Type enemyState, VectorF enemyPosition) : 
		type(enemyType), state(enemyState), position(enemyPosition) { }

	EnemyType type;
	EnemyState::Type state;
	VectorF position;
};

using SpawnDataList = std::vector<SpawnData>;

class SpawnDataBuilder
{
public:
	SpawnDataList buildSpawnData(Formation& formation, EnemyType type, EnemyState::Type state);

	void setRandomPosition(Formation& formation, const AIPathMap* map);

private:
	SpawnDataList buildFormationDataList(const Formation& formation, EnemyState::Type state, EnemyType type) const;

	bool validFormation(const Formation& formation, const AIPathMap* map);
};