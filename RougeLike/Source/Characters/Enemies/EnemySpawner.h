#pragma once

class Map;
class Enemy;

class EnemySpawner
{
public:
	EnemySpawner(Map* map) : mMap(map) { }

	void spawn(Enemy* enemy, float xPositionPercentage);

private:
	Map* mMap;
};