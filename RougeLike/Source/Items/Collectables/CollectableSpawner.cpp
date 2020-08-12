#include "pch.h"
#include "CollectableSpawner.h"
#include "Map/Map.h"


std::queue<CollectableSpawner::SpawnData> CollectableSpawner::getSpawnList(const XMLNode node, const Map* map) const
{
	std::queue<SpawnData> spawnData;

	XMLNode collectablesNode = node.child();
	while (collectablesNode)
	{
		SpawnData data = generateSpawnData(collectablesNode, map);
		spawnData.push(data);

		collectablesNode = collectablesNode.next();
	}

	return spawnData;
}



CollectableSpawner::SpawnData CollectableSpawner::generateSpawnData(const XMLNode node, const Map* map) const
{
	CollectableType type = CollectableType::None;
	type << node.name();

	StringMap attributes = node.attributes();

	int xIncrement = attributes.getInt("xPosition");
	VectorF position = map->randomFloorTile(xIncrement)->rect().Center();

	BasicString id = attributes.at("id");

	return SpawnData(type, position, id);
}