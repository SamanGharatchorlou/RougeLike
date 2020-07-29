#include "pch.h"
#include "CollectablesPool.h"


void CollectablesPool::load()
{
	int defaultPoolSize = 3;

	for (CollectableType type = CollectableType::None + 1; type < CollectableType::Count; type = type + 1)
	{
		addNewObjects(type, defaultPoolSize);
	}
}

Collectable* CollectablesPool::createNewObject(CollectableType type) const
{
	Collectable* collectable = nullptr;

	switch (type)
	{
		break;
	case CollectableType::MeleeWeapon:
		collectable = new WeaponCollectable;
	case CollectableType::Ability:
		collectable = new AbilityCollectable;
	case CollectableType::None:
	case CollectableType::Count:
	default:
		break;
	}

	return collectable;
}



void CollectablesPool::returnObject(Collectable* collectable)
{
	// something needs to clear this object before... handled in effect handler?
	mPool[collectable->type()].push(collectable);
}
