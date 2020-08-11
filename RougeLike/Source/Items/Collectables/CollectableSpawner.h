#pragma once

#include "Items/Collectables/Collectable.h"

class Map;

class CollectableSpawner
{
public:
	struct SpawnData
	{
		SpawnData(CollectableType collectableType, VectorF pos, const BasicString& name) : type(collectableType), position(pos), id(name) { }

		const BasicString id;
		CollectableType type;
		VectorF position;
	};

public:
	std::queue<SpawnData> getSpawnList(const XMLNode node, const Map* map) const;

	SpawnData generateSpawnData(const XMLNode node, const Map* map) const;
};