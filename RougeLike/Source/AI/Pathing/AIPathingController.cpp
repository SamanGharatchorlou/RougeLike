#include "pch.h"
#include "AIPathingController.h"

#include "Map/Map.h"
#include "AIPathMap.h"

#include "Actors/Enemies/Enemy.h"
#include "Actors/Enemies/EnemyStates/EnemyRun.h"


#if DRAW_AI_PATH_COSTMAP
#include "Debug/DebugDraw.h"
#include "Game/Camera/Camera.h"
#include "UI/Elements/UITextBox.h"
#endif


AIPathingController::AIPathingController() : mPathLimit(-1)
{
	// IMPROVEMENT: dynamically adjust this according to framerate drops
#if _DEBUG
	mListSplit = 6;
#else
	mListSplit = 2;
#endif

	mSegmentLength = 200;
	mSegmentIndex = 0;
}


void AIPathingController::clear() 
{ 
	for (int i = 0; i < mPathMaps.size(); i++)
	{
		AIPathMap* pathMap = mPathMaps[i];
		if (pathMap)
		{
			delete pathMap;
			pathMap = nullptr;	
		}
	}
	mPathMaps.clear();
}


void AIPathingController::addMap(const Map* map)
{
	AIPathMap* pathMap = new AIPathMap;
	pathMap->build(map, 2, 2);
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


void AIPathingController::updatePaths(const EnemyList& enemies, float dt)
{
	updateAIPathCostMap(enemies);

	// dynamically adjust the length of the A* pathing calculation
	int minimumFrameCount = 60;
#if DEBUG_CHECK
	minimumFrameCount = 15;
#endif

	int pathingLimit = calculatePathingLimit(minimumFrameCount, dt);

	// split the list up into sections and calculate a single section per frame
	Vector2D<int> range = getCalculationIndexRange(enemies);
	   
	for (int i = range.x; i < range.y; i++)
	{
		Enemy* enemy = enemies[i];
		if (enemy->state() == EnemyState::Run)
		{
			EnemyRun& runState = static_cast<EnemyRun&>(enemy->getStateMachine()->getActiveState());

			// No need to update anything if its about to attack
			if (enemy->hasTarget() && !runState.canAttack())
			{
				runState.updatePath(pathingLimit);
			}
		}
	}
}


Vector2D<int> AIPathingController::getCalculationIndexRange(const EnemyList& enemies)
{
	if (mSegmentLength * mSegmentIndex >= enemies.size())
	{
		mSegmentLength = enemies.size() / mListSplit;
		mSegmentIndex = 0;
	}

	int start = mSegmentLength * mSegmentIndex;
	int end = start + mSegmentLength;
	end = clamp(end, end, (int)enemies.size());

	if ( start >= end || end > enemies.size())
	{
		mSegmentLength = enemies.size() / mListSplit;

		if (mSegmentLength == 0)
			mSegmentLength = enemies.size();

		mSegmentIndex = 0;
		return Vector2D<int>(0, enemies.size());
	}

	mSegmentIndex++;
	return Vector2D<int>(start, end);
}


int AIPathingController::calculatePathingLimit(int minimunFrameCount, float dt)
{
	float maximumTime = 1.0f / (float)minimunFrameCount;
	float spareTime = maximumTime - dt;

	if (spareTime <= 0)
		mPathLimit--;
	else if (spareTime > 0)
		mPathLimit++;

	mPathLimit = clamp(mPathLimit, 5, 50);
	return mPathLimit == 50 ? -1 : mPathLimit;
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

#if DEBUG_CHECK
	DebugPrint(Warning, "No AI path map matches the provied Map object. Has add map been called?\n");
#endif
	return nullptr;
}


void AIPathingController::clearCostMaps()
{
	for (int i = 0; i < mPathMaps.size(); i++)
	{
		mPathMaps[i]->costMapRef().setAllValues(1);
	}
}


void AIPathingController::updateAIPathCostMap(const EnemyList& enemies)
{
	TimerF timer;
	timer.start();
	clearCostMaps();

	for (int i = 0; i < enemies.size(); i++)
	{
		Enemy* enemy = enemies[i];
		Index index = enemy->pathing()->index(enemy->position());

		if (index.isPositive()) // == if valid
		{
			CostMap& costMap = findPathMap(enemy->pathing()->map())->costMapRef();
			costMap[index] += 6;

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
				if (isValid(surroundingIndexsLayer1[i], costMap))
				{
					costMap[surroundingIndexsLayer1[i]] += 2;
				}
			}
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

	const AIPathMap* map = mPathMaps[0];

	for (int y = 0; y < map->yCount(); y++)
	{
		for (int x = 0; x < map->xCount(); x++)
		{
			Index index(x, y);
			VectorF position = map->get(index).rect().TopLeft();

			if (camera->inView(position) && map->get(index).has(CollisionTile::Floor))
			{
				_itoa_s(map->costMap().get(index), text, 10);
				debugRenderText(text, ptSize, position, colour);
			}
		}
	}
}
#endif