#include "pch.h"
#include "Level.h"


Level::Level(int startingLevel) : level(startingLevel), totalExp(0) 
{
	int playerLevel = 1;

	while (playerLevel < startingLevel)
	{
		totalExp += nextLevelExp(playerLevel);
	}
}


void Level::gainExp(int exp)
{
	totalExp += exp;

	if (totalExp >= nextLevelExp(level))
	{
		level++;
	}
}


int Level::nextLevelExp(int currentLevel)
{
	return currentLevel * 1000;
}