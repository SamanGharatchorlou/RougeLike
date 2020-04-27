#include "pch.h"
#include "EnemySpawner.h"

#include "Utilities/Shapes/Shape.h"

#include "Characters/Enemies/EnemyManager.h"
#include "Characters/Enemies/Enemy.h"

#include "Map/Map.h"


void EnemySpawner::spawnPatrollers(Map* map, int xIncrement)
{
	for (unsigned int xPoint = xIncrement; xPoint < 100 - xIncrement; xPoint += xIncrement)
	{
		VectorF position = findSpawnPoint(map, xPoint);
		mEnemies->spawn(EnemyType::Imp, EnemyState::Patrol, position);
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
			DebugPrint(Log, "No valid spawn shape (point count: %d) could be spawned at xPoint %d, No enemies were spawned\n", points.size(), xPoint);
	}

	for (int i = 0; i < points.size(); i++)
	{
		mEnemies->spawn(type, EnemyState::Idle, topLeftPosition + points[i]);
	}
}