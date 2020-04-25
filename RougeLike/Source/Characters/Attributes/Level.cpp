#include "pch.h"
#include "Level.h"


void Level::gainExp(int exp)
{
	currentExp += exp;
	totalExp += exp;

	if (currentExp >= nextLevelExp())
	{
		currentExp = currentExp - nextLevelExp();
		level++;
		levelledUp = true;
	}
}


int Level::nextLevelExp()
{
	return 800 + (level * 200);
}


// Can only query once, after it sets the level up bool back to false
bool Level::didLevelUp()
{
	bool returnValue = levelledUp;

	levelledUp = false;

	return returnValue;
}