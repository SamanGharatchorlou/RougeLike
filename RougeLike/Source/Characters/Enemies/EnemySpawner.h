#pragma once

class Map;
class Enemy;

// TODO: do I need this class?

class EnemySpawner
{
public:
	void spawn(Map* map, Enemy* enemy, int xPositionPercentage);
};