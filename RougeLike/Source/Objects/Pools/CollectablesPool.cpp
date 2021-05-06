#include "pch.h"
#include "CollectablesPool.h"
#include "Items/Collectables/Collectable.h"


Collectable* CollectablesPool::createNewObjects(CollectableType type, int count, int& outSize) const
{
	Collectable* collectable = nullptr;

	DebugPrint(Log, "CollectablesPool has not been implemented\n");
	//switch (type)
	//{
	//	break;
	//case CollectableType::MeleeWeapon:
	//	collectable = new WeaponCollectable;
	//	break;
	//case CollectableType::Ability:
	//	collectable = new AbilityCollectable;
	//	break;
	//case CollectableType::None:
	//case CollectableType::Count:
	//default:
	//	break;
	//}

	return collectable;
}
