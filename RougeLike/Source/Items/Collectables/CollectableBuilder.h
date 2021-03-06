#pragma once

#include "CollectableSpawner.h"
#include "Objects/Pools/CollectablesPool.h"

class CollectableBuilder
{
public:
	void loadPool();
	void clear() { mPool.freeAll(); }

	Collectable* build(CollectableSpawner::SpawnData data);
	void returnCollectable(Collectable* collectable);


private:
	void setIcon(Collectable* collectable, CollectableSpawner::SpawnData data);


protected:
	CollectablesPool mPool;
};