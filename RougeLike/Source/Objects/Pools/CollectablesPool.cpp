#include "pch.h"
#include "CollectablesPool.h"
#include "Items/Collectables/Collectable.h"


Collectable* CollectablesPool::createNewObject(CollectableType type) const
{
	Collectable* collectable = nullptr;

	switch (type)
	{
		break;
	case CollectableType::MeleeWeapon:
		collectable = new WeaponCollectable;
		break;
	case CollectableType::Ability:
		collectable = new AbilityCollectable;
		break;
	case CollectableType::None:
	case CollectableType::Count:
	default:
		break;
	}

	return collectable;
}
