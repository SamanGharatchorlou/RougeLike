#include "pch.h"
#include "AIPathingController.h"

#include "AIPathMap.h"

#include "Actors/Enemies/Enemy.h"
#include "Actors/Enemies/EnemyStates/EnemyRun.h"

#include "Map/Map.h"

#if DRAW_AI_PATH_COSTMAP
#include "Game/Camera/Camera.h"
#include "UI/Elements/UITextBox.h"
#endif


AIPathingController::AIPathingController() : pathUpdateRequests(0), pathUpdateStaggerCounter(0)
{
	updateTimer.start();
}

void AIPathingController::clear() 
{ 
	mPathMaps.clear();  // TODO: delte
	updateTimer.stop();
	pathUpdateRequests = 0;
	pathUpdateStaggerCounter = 0;
}


void AIPathingController::addMap(const Map* map)
{
	// TODO: I call new to allocate some space. how much space is there
	// if the following call build requires 100 units of space, what if I only have 80 for example?
	AIPathMap* pathMap = new AIPathMap;
	pathMap->build(map, 4, 4);
	mPathMaps.push_back(pathMap);
}

AIPathMap* AIPathingController::popMap()
{
	AIPathMap* pathMap = mPathMaps[0];
	delete pathMap;
	mPathMaps[0] = nullptr;

	for (int i = 1; i < mPathMaps.size(); i++)
	{
		mPathMaps[i - 1] = mPathMaps[i];
	}

	mPathMaps.erase(mPathMaps.end() - 1);
	return pathMap;
}

void AIPathingController::updatePaths(std::vector<Enemy*> enemies)
{
	if (pathUpdateStaggerCounter != 0 || pathUpdateRequests)
	{
		recalculateEnemyPaths(enemies);
	}
}



AIPathMap* AIPathingController::pathMap(const Map* map)
{
	for (int i = 0; i < mPathMaps.size(); i++)
	{
		Index index(0, 0);
		if (mPathMaps[i]->tile(index)->rect().TopLeft() == map->tile(index)->rect().TopLeft())
		{

			return mPathMaps[i];
		}
	}

#if _DEBUG
	DebugPrint(Warning, "No AI path map matches the provied Map object. Has add map been called?\n");
#endif
	return nullptr;
}


// If player moves tile or an enemy moves a tile update this bad boi
// TODO: what if the size of active enemies changes during an update?
void AIPathingController::recalculateEnemyPaths(std::vector<Enemy*> enemies)
{
	updateAIPathCostMap(enemies);

	int loopCount = 0;

	for (; pathUpdateStaggerCounter < enemies.size(); pathUpdateStaggerCounter++)
	{
		// Only allow a number of these (expensive) updates per frame, reduce stuttering
		if (loopCount++ >= 3)
			return;

		if (enemies[pathUpdateStaggerCounter]->state() == EnemyState::Run)
		{
			EnemyRun& runState = static_cast<EnemyRun&>(enemies[pathUpdateStaggerCounter]->getStateMachine()->getActiveState());

			// No need to update anything if in attack range
			if (!runState.inAttackRange())
			{
				runState.updatePath();
			}
		}
	}

	if (pathUpdateStaggerCounter == enemies.size())
	{
		pathUpdateStaggerCounter = 0;
		pathUpdateRequests--;

		ASSERT(Warning, pathUpdateRequests >= 0, "Cannot have negative path update requests, count: %d\n", pathUpdateRequests);

		// No reason to have more than 1 request queued up after completing a request
		pathUpdateRequests = clamp(pathUpdateStaggerCounter, 0, 1);
	}
}

void AIPathingController::clearCostMaps()
{
	for (int i = 0; i < mPathMaps.size(); i++)
	{
		mPathMaps[i]->costMapRef().setAllValues(1);
	}
}


void AIPathingController::updateAIPathCostMap(std::vector<Enemy*> enemies)
{
	clearCostMaps();

	for (int i = 0; i < enemies.size(); i++)
	{
		Enemy* enemy = enemies[i];
		CostMap& costMap = findPathMap(enemy->pathing()->map())->costMapRef();
		
		// Current tile
		Index index = enemy->pathing()->index(enemy->position());
		costMap[index] += 5;

		// Immediate surrounding tiles
		Index surroundingIndexsLayer1[8]{
			Index(index + Index(-1,-1)),
			Index(index + Index(+0,-1)),
			Index(index + Index(+1,-1)),

			Index(index + Index(-1,0)),
			Index(index + Index(+1,0)),

			Index(index + Index(-1,+1)),
			Index(index + Index(+0,+1)),
			Index(index + Index(+1,+1))
		};

		for (int i = 0; i < 8; i++)
		{
			costMap[surroundingIndexsLayer1[i]] += 1;
		}
	}
}


AIPathMap* AIPathingController::findPathMap(const AIPathMap* pathMap)
{
	for (int i = 0; i < mPathMaps.size(); i++)
	{
		if (pathMap == mPathMaps[i])
			return mPathMaps[i];
	}

	DebugPrint(Warning, "This path map should no longer exist!\n");
	return nullptr;
}


#if DRAW_AI_PATH_COSTMAP // TODO: get current map
void AIPathingController::drawCostMap()
{
	int ptSize = 16;
	RenderColour colour = RenderColour::Red;

	char text[3];
	Camera* camera = Camera::Get();

	for (int y = 0; y < mPathMaps[0].yCount(); y++)
	{
		for (int x = 0; x < mPathMaps[0].xCount(); x++)
		{
			Index index(x, y);
			VectorF position = mPathMaps[0][index].rect().TopLeft();

			if (camera->inView(position) && mPathMaps[0][index].has(CollisionTile::Floor))
			{
				_itoa_s(mPathMaps[0].costMap().get(index), text, 10);
				debugRenderText(text, ptSize, position, colour);
			}
		}
	}
}
#endif