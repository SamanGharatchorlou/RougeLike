#pragma once

#include "Items/Spawner.h"
#include "Actors/Enemies/Types/EnemyTypes.h"
#include "Actors/Enemies/EnemyStates/EnemyState.h"
#include "Utilities/Shapes/Shape.h"


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
	std::vector<SpawnData> getspawnList(const XMLParser& parser, const Map* map) const;


private:
	std::vector<SpawnData> generateSpawnData(const Map* map, Type spawnType, const Attributes& attributes) const;

	std::vector<SpawnData> spawnPatrollers(const Map* map, const Attributes& attributes) const;
	std::vector<SpawnData> spawnShape(const Map* map, const Attributes& attributes) const;

	Type stringToType(const std::string& spawnType) const;

	std::vector<SpawnData> buildSpawnList(const PointList& points, EnemyState::Type state, EnemyType type) const;
	void findValidShape(Shape& shape, const Map* map, int xPosition) const;
	bool isValid(const Shape& shape, const Map* map) const;

	Shape pickRandomShape() const;
};