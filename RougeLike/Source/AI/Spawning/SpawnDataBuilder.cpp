#include "pch.h"
#include "SpawnDataBuilder.h"

#include "AI/Pathing/AIPathMap.h"
#include "Utilities/Shapes/Formation.h"



SpawnDataList SpawnDataBuilder::buildSpawnData(Formation& formation, EnemyType type, EnemyState::Type state)
{
	SpawnDataList spawnList;
	const PointList& formationPosition = formation.points();

	for (int i = 0; i < formationPosition.size(); i++)
	{
		SpawnData data(type, state, formationPosition.at(i));
		spawnList.push_back(data);
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

std::vector<SpawnData> SpawnDataBuilder::buildFormationDataList(const Formation& formation, EnemyState::Type state, EnemyType type) const
{
	SpawnDataList spawnList;
	const PointList& formationPosition = formation.points();

	for (int i = 0; i < formationPosition.size(); i++)
	{
		SpawnData data(type, state, formationPosition.at(i));
		spawnList.push_back(data);
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