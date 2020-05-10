#pragma once

#include "Objects/Attributes/Property.h"

class Level : public Property
{
public:
	Level() : level(1), currentExp(0), totalExp(0), levelledUp(false) { }

	void gainExp(int exp);

	int getCurrentExp() const { return currentExp; }
	float value() const override { return (float)level; }

	int nextLevelExp();

	bool didLevelUp();


private:
	int level;
	int currentExp;
	int totalExp;

	// IDEA: could turn this into a stack and pop off level up bools?
	bool levelledUp;
};