#include "pch.h"
#include "AISpawnController.h"

#include "AI/Pathing/AIPathMap.h"
#include "Actors/Enemies/EnemyManager.h"

#include "Utilities/Shapes/Point.h"
#include "Utilities/Shapes/Quadrilateral.h"

AISpawnController::AISpawnController() { }

void AISpawnController::init()
{
	mFactory.initSpawnInfo();
}


void AISpawnController::spawnUnspawnedEnemies(EnemyManager* enemyManager)
{
	if (mSpawnList.hasData())
	{
		int counter = 0;
		while (mSpawnList.hasData() && counter < mSpawnList.mSpawnsPerFrame)
		{
			counter++;

			const SpawnData data = mSpawnList.mData.pop();
			Enemy* enemy = mFactory.buildEnemy(data, mSpawnList.mMap);
			enemyManager->spawnEnemy(enemy);
		}
	}
}


void AISpawnController::initSpawningEnemies(const AIPathMap* map)
{
	mSpawnList.init(map);

	// spawn patrollers
	int spacing = getPatrollerSpacing(map->level());
	for (unsigned int xPoint = 5; xPoint <= 95; xPoint += spacing)
	{
		VectorF position = map->randomFloorTile(xPoint)->rect().Center();
		Point patrolPoint(position);
		mSpawnList.add(mSpawnData.buildSpawnData(patrolPoint, EnemyType::Devil, EnemyState::Patrol));
	}

	// spawn formations
	int formationCount = clamp(map->level() + 1, 0, 8);
	for (int i = 0; i < formationCount; i++)
	{
		EnemyType type = i % 2 == 0 ? EnemyType::Orc : EnemyType::Devil;
		mSpawnList.add(spawnRandomQuad(type, EnemyState::Idle, map));
	}

	mFactory.topUpPool(mSpawnList);

	// Running at max speed we have ~4s before we see any enemies
	// Spawning everything over 100 frames seem safe enough while
	// reducing the load per frame to remove the jank across levels
	mSpawnList.setSpawnPerFrames(100);
}


void AISpawnController::returnEnemy(Enemy* enemy)
{
	mFactory.returnEnemy(enemy);
}


void AISpawnController::loadSpawnPool()
{
	mFactory.loadPools();
}


AISpawnController::~AISpawnController()
{
	clear();
}


void AISpawnController::clear()
{
	mFactory.clear();
}


// -- Private Functions -- //

Queue<SpawnData> AISpawnController::spawnRandomQuad(EnemyType type, EnemyState::Type state, const AIPathMap* map)
{
	Vector2D<int> formationSize = mSpawnData.getRandomFormationSize(map->level());
	Quadrilateral formation(VectorF(), formationSize.x, formationSize.y, 40.0f);
	setRandomPosition(formation, map);

	return mSpawnData.buildSpawnData(formation, type, state);
}


void AISpawnController::setRandomPosition(Formation& formation, const AIPathMap* map)
{
	while (!formation.isValid(map))
	{
		VectorF randomFloorPosition = map->randomFloorTile()->rect().Center();
		formation.setPosition(randomFloorPosition);
	}
}


int AISpawnController::getPatrollerSpacing(int mapLevel) const
{
	int spacing = 5;

	if (mapLevel > 2)  spacing = 4;
	if (mapLevel > 5)  spacing = 3;
	if (mapLevel > 7)  spacing = 2;
	if (mapLevel > 10) spacing = 1;

	return spacing;
}

