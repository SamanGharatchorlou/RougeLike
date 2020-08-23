#pragma once

#include "Utilities/ObjectPool.h"
#include "Items/Collectables/Collectable.h"

enum class CollectableType;


class CollectablesPool : public ObjectPool<Collectable, CollectableType>
{
private:
	Collectable* createNewObject(CollectableType type) const override;
};

