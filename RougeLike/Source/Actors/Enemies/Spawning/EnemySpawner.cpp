#include "pch.h"
#include "EnemySpawner.h"

#include "Utilities/Shapes/Square.h"
#include "Utilities/Shapes/Circle.h"

#include "Actors/Enemies/EnemyManager.h"
#include "Actors/Enemies/Enemy.h"

#include "Map/Map.h"


void EnemySpawner::wipeEnemies()
{
	//mEnemies->clearAllEnemies();
}

std::vector<SpawnData> EnemySpawner::getspawnList(const XMLParser& parser, const Map* map)
{
	std::vector<SpawnData> spawnList;

	xmlNode root = parser.rootNode();
	xmlNode spawnerLevelNode = root->first_node("Spawning");
	if (spawnerLevelNode)
	{
		xmlNode spawnNode = spawnerLevelNode->first_node();
		while (spawnNode)
		{
			Type spawnType = stringToType(spawnNode->name());
			Attributes attributes = parser.attributes(spawnNode);

			std::vector<SpawnData> list = generateSpawnData(map, spawnType, attributes);
			spawnList.reserve(spawnList.size() + list.size());
			spawnList.insert(spawnList.end(), list.begin(), list.end());

			spawnNode = spawnNode->next_sibling();
		}
	}

	return spawnList;
}


std::vector<SpawnData> EnemySpawner::generateSpawnData(const Map* map, Type spawnType, const Attributes& attributes)
{
	if (spawnType == Type::Patrol)
		 return spawnPatrollers(map, attributes);
}


EnemySpawner::Type EnemySpawner::stringToType(const std::string& spawnType)
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


void EnemySpawner::spawnPatrollers(const Map* map, int xIncrement, EnemyType type)
{
	for (unsigned int xPoint = xIncrement; xPoint < 100 - xIncrement; xPoint += xIncrement)
	{
		VectorF position = findSpawnPoint(map, xPoint);
		//mEnemies->spawn(type, EnemyState::Patrol, position);
	}
}

std::vector<SpawnData> EnemySpawner::spawnPatrollers(const Map* map, const Attributes& attributes)
{
	EnemyType enemyType = stringToEnemyType(attributes.getString("type"));
	int xIncrement = attributes.getInt("xIncrement");

	std::vector<SpawnData> spawnList;
	for (unsigned int xPoint = 5; xPoint <= 95; xPoint += xIncrement)
	{
		VectorF position = findSpawnPoint(map, xPoint);
		spawnList.push_back(SpawnData(enemyType, EnemyState::Patrol, position));
	}

	return spawnList;
}


void EnemySpawner::spawnShape(const Map* map, int xPoint, Shape shape, EnemyType type)
{
	PointList points = shape.points();

	VectorF topLeftPosition = findSpawnPoint(map, xPoint);

	int sanityCounter = 0;
	for (int i = 0; i < points.size(); i++)
	{
		VectorF shapePosition = topLeftPosition + points[i];

		Index index = map->index(shapePosition);

		// If any point is invalid, start again at another point
		//if (!map->floorCollisionTile(index))
		//{
		//	i = 0;
		//	topLeftPosition = findSpawnPoint(map, xPoint);
		//	sanityCounter++;
		//}

		// Just in case no spawn point can be found, prevent infinite loop
		if (sanityCounter > 50)
		{
			DebugPrint(Log, "No valid spawn shape (point count: %d) could be spawned at xPoint %d, No enemies were spawned\n", points.size(), xPoint);
			spawnShape(map, xPoint + 1, shape, type);
			return;
		}
			
	}

	for (int i = 0; i < points.size(); i++)
	{
		//mEnemies->spawn(type, EnemyState::Idle, topLeftPosition + points[i]);
	}
}



Shape EnemySpawner::pickRandomShape()
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
