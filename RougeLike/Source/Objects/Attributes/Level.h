#pragma once

#include "Objects/Properties/PropertyBase.h"

class Level : public PropertyBase
{
public:
	Level() : PropertyBase(PropertyType::Level), level(1), currentExp(0), totalExp(0), levelledUp(false) { }

	void gainExp(int exp);

	int getCurrentExp() const { return currentExp; }
	float value() const { return (float)level; }

	int nextLevelExp();

	bool didLevelUp();


private:
	int level;
	int currentExp;
	int totalExp;

	// IDEA: could turn this into a stack and pop off level up bools?
	bool levelledUp;
};