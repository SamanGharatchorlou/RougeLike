#include "pch.h"
#include "EnemySpawner.h"

#include "Utilities/Shapes/Square.h"
#include "Utilities/Shapes/Circle.h"

#include "Actors/Enemies/EnemyManager.h"
#include "Actors/Enemies/Enemy.h"

#include "Map/Map.h"





//
//// Decorations
//void LevelManager::decorate(Map* map, const std::string& section)
//{
//	std::string fileName = "Level" + std::to_string(mLevel);
//	std::string path = FileManager::Get()->findFile(FileManager::Config_Map, fileName);
//
//	if (!path.empty())
//	{
//		// Decorations
//		XMLParser parser(path);
//		DecorMap decorations = getLevelDecorInfo(parser, section);
//		MapDecorator decorator;
//		decorator.addDecor(decorations, map->getData());
//
//		// Traps
//		DecorMap trapInfo = getLevelTrapInfo(parser, section);
//		setTrapInfo(map, trapInfo);
//	}
//}


void EnemySpawner::wipeEnemies()
{
	//mEnemies->clearAllEnemies();
}

std::vector<SpawnData> EnemySpawner::getspawnList(Map* map, int level)
{
	std::vector<SpawnData> spawnList;

	std::string fileName = "Level" + std::to_string(level);
	std::string path = FileManager::Get()->findFile(FileManager::Config_Map, fileName);
	if (!path.empty())
	{
		XMLParser parser(path);
		xmlNode root = parser.rootNode();
		xmlNode spawnerLevelNode = root->first_node("Spawning");
		if (spawnerLevelNode)
		{
			xmlNode spawnNode = spawnerLevelNode->first_node();
			while (spawnNode)
			{
				Type spawnType = stringToType(spawnNode->name());
				Attributes attributes = parser.attributes(spawnNode);

				std::vector<SpawnData> list = spawnEnemies(map, spawnType, attributes);
				spawnList.reserve(spawnList.size() + list.size());
				spawnList.insert(spawnList.end(), list.begin(), list.end());

				spawnNode = spawnNode->next_sibling();
			}
		}
	}

	return spawnList;
}


std::vector<SpawnData> EnemySpawner::spawnEnemies(Map* map, Type spawnType, const Attributes& attributes)
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


void EnemySpawner::spawnLevel(Map* map, int level)
{
	switch (level)
	{
	case 1:
	{
		level1(map);
		break;
	}
	case 2:
	{
		level2(map);
		break;
	}
	case 3:
	{
		level3(map);
		break;
	}
	case 4:
	{

	}
	default:
		break;
	}
}


void EnemySpawner::spawnPatrollers(Map* map, int xIncrement, EnemyType type)
{
	for (unsigned int xPoint = xIncrement; xPoint < 100 - xIncrement; xPoint += xIncrement)
	{
		VectorF position = findSpawnPoint(map, xPoint);
		//mEnemies->spawn(type, EnemyState::Patrol, position);
	}
}

std::vector<SpawnData> EnemySpawner::spawnPatrollers(Map* map, const Attributes& attributes)
{
	EnemyType enemyType = stringToEnemyType(attributes.getString("type"));
	int xIncrement = attributes.getInt("xIncrement");

	std::vector<SpawnData> spawnList;
	spawnList.reserve(90 / xIncrement);

	for (unsigned int xPoint = 5; xPoint <= 95; xPoint += xIncrement)
	{
		VectorF position = findSpawnPoint(map, xPoint);
		spawnList.push_back(SpawnData(enemyType, EnemyState::Patrol, position));
	}

	return spawnList;
}


void EnemySpawner::spawnShape(Map* map, int xPoint, Shape shape, EnemyType type)
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



// Level spawning functions
void EnemySpawner::level1(Map* map)
{
	spawnPatrollers(map, 5, EnemyType::Devil);

	//spawnShape(map, 30, pickRandomShape(), EnemyType::Imp);
	//spawnShape(map, 60, pickRandomShape(), EnemyType::Angel);
}


void EnemySpawner::level2(Map* map)
{
	spawnPatrollers(map, 7, EnemyType::Imp);

	spawnShape(map, 20, pickRandomShape(), EnemyType::Imp);
	spawnShape(map, 50, pickRandomShape(), EnemyType::Angel);
	spawnShape(map, 75, pickRandomShape(), EnemyType::Imp);
}


void EnemySpawner::level3(Map* map)
{
	spawnPatrollers(map, 10, EnemyType::Angel);

	spawnShape(map, 20, pickRandomShape(), EnemyType::Imp);
	spawnShape(map, 50, pickRandomShape(), EnemyType::Angel);
	spawnShape(map, 60, pickRandomShape(), EnemyType::Imp);
	spawnShape(map, 80, pickRandomShape(), EnemyType::Angel);
}
