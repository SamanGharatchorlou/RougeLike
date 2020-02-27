#pragma once

class Level
{
public:
	Level() : level(1), currentExp(0), totalExp(0) { }

	void gainExp(int exp);

	int getCurrentExp() const { return currentExp; }
	int getLevel() const { return level; }

	int nextLevelExp();

	bool didLevelUp();

private:
	int level;
	int currentExp;
	int totalExp;

	// IDEA: could turn this into a stack and pop off level up bools?
	bool levelledUp;
};