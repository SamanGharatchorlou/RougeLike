#pragma once


#include "Items/Spawner.h"
#include "Items/Collectables/Collectable.h"


class CollectableSpawner : public Spawner
{
public:
	struct SpawnData
	{
		SpawnData(CollectableType collectableType, VectorF pos, const std::string& name) : type(collectableType), position(pos), id(name) { }
		CollectableType type;
		VectorF position;
		const std::string id;
	};

public:
	std::queue<SpawnData> getSpawnList(const XMLParser& parser, const Map* map);

	SpawnData generateSpawnData(const XMLNode& node, const Map* map);
};