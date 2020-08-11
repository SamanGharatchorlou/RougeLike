#pragma once

#include "Objects/Properties/Attributes/Attribute.h"


class Level : public Attribute
{
public:
	Level() : level(1), currentExp(0), totalExp(0), levelledUp(false) { }

	void gainExp(int exp);

	int getCurrentExp() const { return currentExp; }
	const float value() const override { return (float)level; }
	void setValue(float value) override { level = (int)value; }

	int nextLevelExp();

	bool didLevelUp();


private:
	int level;
	int currentExp;
	int totalExp;

	// IDEA: could turn this into a stack and pop off level up bools?
	bool levelledUp;
};