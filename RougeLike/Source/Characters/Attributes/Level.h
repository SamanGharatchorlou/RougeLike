#pragma once

class Level
{
public:
	Level() : level(1), totalExp(0) { }
	Level(int startingLevel);

	void gainExp(int exp);

	int getLevel() const { return level; }

	int nextLevelExp(int currentLevel);

private:
	int level;
	int totalExp;
};