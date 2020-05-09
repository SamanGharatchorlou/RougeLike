#include "pch.h"
#include "StatManager.h"


void StatManager::init(PlayerPropertyBag* propertyBag)
{
	bag = propertyBag;
}


void StatManager::gainExp(int exp)
{
	Level* playerLevel = static_cast<Level*>(bag->get(PropertyType::Level));
	playerLevel->gainExp(exp);
}