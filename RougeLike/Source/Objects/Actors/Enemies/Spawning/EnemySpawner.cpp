#include "pch.h"
#include "EnemySpawner.h"

#include "Utilities/Shapes/Square.h"
#include "Utilities/Shapes/Circle.h"

#include "Objects/Actors/Enemies/EnemyManager.h"
#include "Objects/Actors/Enemies/Enemy.h"

#include "Map/Map.h"


void EnemySpawner::spawnLevel(Map* map, int level)
{
	switch (level)
	{
	case 1:
	{
		//level1(map);
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
		mEnemies->spawn(type, EnemyState::Patrol, position);
	}
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
		if (!map->floorCollisionTile(index))
		{
			i = 0;
			topLeftPosition = findSpawnPoint(map, xPoint);
			sanityCounter++;
		}

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
		mEnemies->spawn(type, EnemyState::Idle, topLeftPosition + points[i]);
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
	spawnPatrollers(map, 10, EnemyType::Imp);

	spawnShape(map, 30, pickRandomShape(), EnemyType::Imp);
	spawnShape(map, 60, pickRandomShape(), EnemyType::Angel);
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
