#include "pch.h"
#include "AIPathingController.h"

#include "AIPathMap.h"

#include "Actors/Enemies/Enemy.h"
#include "Actors/Enemies/EnemyStates/EnemyRun.h"

#include "Map/Map.h"

#include "Debug/FrameRateController.h"

#if DRAW_AI_PATH_COSTMAP
#include "Game/Camera/Camera.h"
#include "UI/Elements/UITextBox.h"
#endif


AIPathingController::AIPathingController() : pathUpdateRequests(0), pathUpdateStaggerCounter(0), mPathLimit(-1)
{
	updateTimer.start();

	// Split enemy list calculations over 5 frames, for now this is more than enough
	// IMPROVEMENT: dynamically adjust this according to framerate drops
	mListSplit = 5;

	mSegmentLength = 10;
	mSegmentIndex = 0;
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


void AIPathingController::updatePaths(const std::vector<Enemy*>& enemies, float dt)
{
	updateAIPathCostMap(enemies);

	// dynamically adjust the length of the A* pathing calculation
	int minimumFrameCount = 60;
#if _DEBUG
	minimumFrameCount = 15;
#endif

	float pathingLimit = calculatePathingLimit(minimumFrameCount, dt);

	// split the list up into sections and calculate a single section per frame
	Vector2D<int> range = getCalculationIndexRange(enemies);
	   
	for (int i = range.x; i < range.y; i++)
	{
		Enemy* enemy = enemies[i];
		if (enemy->state() == EnemyState::Run)
		{
			EnemyRun& runState = static_cast<EnemyRun&>(enemy->getStateMachine()->getActiveState());

			// No need to update anything if its about to attack
			if (!runState.canAttack())
			{
				runState.updatePath(pathingLimit);
			}
		}
	}
}


Vector2D<int> AIPathingController::getCalculationIndexRange(const std::vector<Enemy*>& enemies)
{
	if (mSegmentLength * mSegmentIndex >= enemies.size())
	{
		mSegmentLength = enemies.size() / mListSplit;
		mSegmentIndex = 0;
	}

	int start = mSegmentLength * mSegmentIndex;
	int end = start + mSegmentLength;
	end = clamp(end, end, (int)enemies.size());

	if ( start < 0	|| start >= end ||
		 end < 0	|| end > enemies.size())
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

#if _DEBUG
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


void AIPathingController::updateAIPathCostMap(const std::vector<Enemy*>& enemies)
{
	TimerF timer;
	timer.start();
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
			if (isValid(surroundingIndexsLayer1[i], costMap))
			{
				costMap[surroundingIndexsLayer1[i]] += 1;
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