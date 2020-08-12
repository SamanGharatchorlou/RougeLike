#include "pch.h"
#include "EnemySpawner.h"

#include "Utilities/Shapes/Square.h"
#include "Utilities/Shapes/Circle.h"

#include "Actors/Enemies/EnemyManager.h"
#include "Actors/Enemies/Enemy.h"

#include "Map/Map.h"


std::vector<SpawnData> EnemySpawner::getspawnList(const XMLNode spawnNode, const Map* map) const
{
	std::vector<SpawnData> spawnList;

	XMLNode node = spawnNode.child();
	while (node)
	{
		Type spawnType = stringToType(node.name());
		StringMap attributes = node.attributes();

		std::vector<SpawnData> list = generateSpawnData(map, spawnType, attributes);
		spawnList.reserve(spawnList.size() + list.size());
		spawnList.insert(spawnList.end(), list.begin(), list.end());

		node = node.next();
	}

	return spawnList;
}


std::vector<SpawnData> EnemySpawner::generateSpawnData(const Map* map, Type spawnType, const StringMap& attributes) const
{
	if (spawnType == Type::Patrol)
		return spawnPatrollers(map, attributes);
	else if (spawnType == Type::Shape)
		return spawnShape(map, attributes);
}


std::vector<SpawnData> EnemySpawner::spawnPatrollers(const Map* map, const StringMap& attributes) const
{
	EnemyType enemyType = EnemyType::None;
	enemyType << attributes.at("type");

	int xIncrement = toInt(attributes.at("xIncrement"));

	PointList pointList;
	for (unsigned int xPoint = 5; xPoint <= 95; xPoint += xIncrement)
	{
		VectorF position = map->randomFloorTile(xPoint)->rect().Center();
		pointList.push_back(position);
	}

	std::vector<SpawnData> spawnList = buildSpawnList(pointList, EnemyState::Patrol, enemyType);
	return spawnList;
}


std::vector<SpawnData> EnemySpawner::spawnShape(const Map* map, const StringMap& attributes) const
{
	EnemyType enemyType = EnemyType::None;
	enemyType << attributes.at("type");

	int xPosition = toInt(attributes.at("xPosition"));

	PointList points;
	if (attributes.at("shape") == "Circle")
	{
		int radius = map->tile(Index(0, 0))->rect().Width();
		int pointCount = toInt(attributes.at("points"));
		
		Circle circle(VectorF(), radius, pointCount);
		findValidShape(circle, map, xPosition);
		points = circle.points();
	}

	std::vector<SpawnData> spawnList = buildSpawnList(points, EnemyState::Idle, enemyType);
	return spawnList;
}


void EnemySpawner::findValidShape(Shape& shape, const Map* map, int xPosition) const
{
	bool validShapePosition = false;

	while (!validShapePosition)
	{
		VectorF position = map->randomFloorTile(xPosition)->rect().Center();
		shape.setPosition(position);

		validShapePosition = isValid(shape, map);
	}
}


bool EnemySpawner::isValid(const Shape& shape, const Map* map) const
{
	PointList points = shape.points();

	for (int i = 0; i < points.size(); i++)
	{
		if (!map->isValidPosition(points[i]))
			return false;
	}

	return true;
}


std::vector<SpawnData> EnemySpawner::buildSpawnList(const PointList& points, EnemyState::Type state, EnemyType type) const
{
	std::vector<SpawnData> spawnList;

	for (int i = 0; i < points.size(); i++)
	{
		SpawnData data(type, EnemyState::Idle, points[i]);
		spawnList.push_back(data);
	}

	return spawnList;
}


Shape EnemySpawner::pickRandomShape() const
{
	int randomNumber = randomNumberBetween(0, 2);

	if (randomNumber == 0)
	{
		float length = 50.0f;
		return Square(VectorF(), length);
	}
	else if (randomNumber == 1)
	{
		float length = 60.0f;
		int pointCount = randomNumberBetween(6, 12);

		return Circle(VectorF(), length, pointCount);
	}
	else
	{
		DebugPrint(Log, "picking this random shape number %d not connected to a shape. Returning empty shape.\n");
		return Shape();
	}
}


// TODO: replace with << and/or >>
EnemySpawner::Type EnemySpawner::stringToType(const BasicString& spawnType) const
{
	if (spawnType == "Patrol")
		return Type::Patrol;
	else if (spawnType == "Shape")
		return Type::Shape;
	else
	{
		DebugPrint(Warning, "Spawn type string '%s' not recognised, no spawn type set\n", spawnType.c_str());
		return Type::None;
	}
}