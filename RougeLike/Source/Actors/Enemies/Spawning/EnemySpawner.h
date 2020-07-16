#pragma once

#include "Items/Spawner.h"
#include "Actors/Enemies/Types/EnemyTypes.h"
#include "Actors/Enemies/EnemyStates/EnemyState.h"

class EnemyManager;
class Shape;


struct SpawnData
{
	SpawnData(EnemyType enemyType, EnemyState::Type enemyState, VectorF enemyPosition) : 
		type(enemyType), state(enemyState), position(enemyPosition) { }

	EnemyType type;
	EnemyState::Type state;
	VectorF position;
};


class EnemySpawner : public Spawner
{
public:
	enum class Type
	{
		None,
		Patrol,
		Shape
	};

public:
	EnemySpawner() { }

	void wipeEnemies();
	std::vector<SpawnData> getspawnList(Map* map, int level);

	void spawnLevel(Map* map, int level);

	void spawnPatrollers(Map* map, int xIncrement, EnemyType type);

	void spawnShape(Map* map, int xPoint, Shape shape, EnemyType type);
	//void spawnShape(Map* map, const Attributes& attributes);


private:
	std::vector<SpawnData> spawnEnemies(Map* map, Type spawnType, const Attributes& attributes);
	std::vector<SpawnData> spawnPatrollers(Map* map, const Attributes& attributes);

	Type stringToType(const std::string& spawnType);

	void level1(Map* map);
	void level2(Map* map);
	void level3(Map* map);

	Shape pickRandomShape();

};