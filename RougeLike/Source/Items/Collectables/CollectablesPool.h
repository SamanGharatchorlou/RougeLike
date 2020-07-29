#pragma once

#include "Utilities/ObjectPool.h"
#include "Collectable.h"

class Collectable;


class CollectablesPool : public ObjectPool<Collectable, CollectableType>
{
public:
	CollectablesPool() { }

	void load() override;

	void returnObject(Collectable* collectable) override;

private:
	Collectable* createNewObject(CollectableType type) const override;
};

