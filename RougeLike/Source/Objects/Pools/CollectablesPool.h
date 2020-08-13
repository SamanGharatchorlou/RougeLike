#pragma once

#include "Utilities/ObjectPool.h"

class Collectable;
enum class CollectableType;


class CollectablesPool : public ObjectPool<Collectable, CollectableType>
{
private:
	Collectable* createNewObject(CollectableType type) const override;
};

