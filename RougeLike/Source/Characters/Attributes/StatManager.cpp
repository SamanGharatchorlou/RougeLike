#include "pch.h"
#include "StatManager.h"


void StatManager::init(PlayerPropertyBag* propertyBag)
{
	bag = propertyBag;
}


void StatManager::gainExp(int exp)
{
	Level playerLevel = bag->pLevel.get();

	int currentLevel = playerLevel.getLevel();

	playerLevel.gainExp(exp);

	if (playerLevel.getLevel() != currentLevel)
	{
		// player has leveled up! increase other stats
		bag->pDefence.get() += bag->pDefenceUp.get();

		bag->pHealth.get().increaseMax(bag->pHealthUp.get());

		bag->pAttackDmg.get() += bag->pAttackDmgUp.get();

		bag->pAttackSpd.get() += bag->pAttackSpdUp.get();

		bag->pMovementSpd.get() += bag->pMovementSpdUp.get();
	}
}