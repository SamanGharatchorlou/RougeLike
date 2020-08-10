#pragma once


#include "Items/Spawner.h"
#include "Items/Collectables/Collectable.h"


class CollectableSpawner : public Spawner
{
public:
	struct SpawnData
	{
		SpawnData(CollectableType collectableType, VectorF pos, const BasicString& name) : type(collectableType), position(pos), id(name) { }
		CollectableType type;
		VectorF position;
		const BasicString id;
	};

public:
	std::queue<SpawnData> getSpawnList(const XMLNode node, const Map* map) const;

	SpawnData generateSpawnData(const XMLNode node, const Map* map) const;
};