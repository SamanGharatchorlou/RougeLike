#include "pch.h"
#include "CollectableSpawner.h"



std::queue<CollectableSpawner::SpawnData> CollectableSpawner::getSpawnList(const XMLParser& parser, const Map* map)
{
	std::queue<SpawnData> spawnData;

	xmlNode rootNode = parser.rootNode();
	xmlNode collectablesNode = rootNode->first_node("Collectables");

	if (collectablesNode)
	{
		xmlNode collectableNode = collectablesNode->first_node();
		while (collectableNode)
		{
			SpawnData data = generateSpawnData(XMLNode(collectableNode), map);
			spawnData.push(data);

			collectableNode = collectableNode->next_sibling();
		}
	}

	return spawnData;
}



CollectableSpawner::SpawnData CollectableSpawner::generateSpawnData(const XMLNode& node, const Map* map)
{
	CollectableType type = CollectableType::None;
	type << node.name();

	Attributes attributes = node.attributes();

	int xIncrement = attributes.getInt("xPosition");
	VectorF position = findSpawnPoint(map, xIncrement);

	BasicString id = attributes.getString("id");

	return SpawnData(type, position, id);
}