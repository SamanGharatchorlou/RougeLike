#include "pch.h"
#include "CollectableSpawner.h"



std::queue<CollectableSpawner::SpawnData> CollectableSpawner::getSpawnList(const XMLNode node, const Map* map) const
{
	std::queue<SpawnData> spawnData;

	XMLNode collectablesNode = node.child("Collectables");
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

	StringMap attributes = node.nodeAttributes();

	int xIncrement = attributes.getInt("xPosition");
	VectorF position = findSpawnPoint(map, xIncrement);

	BasicString id = attributes.at("id");

	return SpawnData(type, position, id);
}