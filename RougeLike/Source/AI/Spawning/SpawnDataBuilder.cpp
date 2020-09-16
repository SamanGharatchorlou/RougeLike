#include "pch.h"
#include "SpawnDataBuilder.h"

#include "AI/Pathing/AIPathMap.h"
#include "Utilities/Shapes/Formation.h"



Queue<SpawnData> SpawnDataBuilder::buildSpawnData(Formation& formation, EnemyType type, EnemyState::Type state)
{
	Queue<SpawnData> spawnList;
	const PointList& formationPosition = formation.points();

	for (int i = 0; i < formationPosition.size(); i++)
	{
		SpawnData data(type, state, formationPosition.at(i));
		spawnList.push(data);
	}

	return spawnList;
}

void SpawnDataBuilder::setRandomPosition(Formation& formation, const AIPathMap* map)
{
	while (!validFormation(formation, map))
	{
		VectorF randomFloorPosition = map->randomFloorTile()->rect().Center();
		formation.setPosition(randomFloorPosition);
	}
}

Queue<SpawnData> SpawnDataBuilder::buildFormationDataList(const Formation& formation, EnemyState::Type state, EnemyType type) const
{
	Queue<SpawnData> spawnList;
	const PointList& formationPosition = formation.points();

	for (int i = 0; i < formationPosition.size(); i++)
	{
		SpawnData data(type, state, formationPosition.at(i));
		spawnList.push(data);
	}

	return spawnList;
}


bool SpawnDataBuilder::validFormation(const Formation& formation, const AIPathMap* map)
{
	const PointList& positions = formation.points();

	for (const VectorF& position : positions)
	{
		if (!map->isValidPosition(position))
			return false;
	}

	return true;
}


Vector2D<int> SpawnDataBuilder::getRandomFormationSize(int mapLevel) const
{
	int minimun = 2;
	int maximum = clamp(5 + mapLevel, minimun, 10);

	int xRandom = randomNumberBetween(minimun, maximum);
	int yRandom = randomNumberBetween(minimun, maximum);

	return Vector2D<int>(xRandom, yRandom);
}