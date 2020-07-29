#include "pch.h"
#include "AIController.h"

#include "Enemy.h"
#include "EnemyStates/EnemyRun.h"

#include "Map/Map.h"


#if DRAW_AI_PATH_COSTMAP
#include "Game/Camera.h"
#include "UI/Elements/UITextBox.h"
#endif

AIController::AIController() : pathUpdateRequests(0), pathUpdateStaggerCounter(0)
{
	updateTimer.start();
}



void AIController::loadAIPathMap(const Map* map)
{
	mPathMap.build(map, 4, 4);
}

void AIController::updatePaths(std::vector<Enemy*>& enemies)
{
	if (pathUpdateStaggerCounter != 0 || pathUpdateRequests)
	{
		recalculateEnemyPaths(enemies);
	}
}



// If player moves tile or an enemy moves a tile update this bad boi
// TODO: what if the size of active enemies changes during an update?
void AIController::recalculateEnemyPaths(std::vector<Enemy*>& enemies)
{
	mPathMap.costMapRef().setAllValues(1);
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


void AIController::updateAIPathCostMap(std::vector<Enemy*>& enemies)
{
	Grid<int>& AICostMap = mPathMap.costMapRef();
	AICostMap.setAllValues(1);

	for (int i = 0; i < enemies.size(); i++)
	{
		// Current tile
		Index index = mPathMap.index(enemies[i]->position());
		AICostMap[index] += 10;

		// Immediate surrounding tiles
		Index surroundingIndexsLayer1[8]{
			Index(index + Index(-1,-1)),
			Index(index + Index(+0, -1)),
			Index(index + Index(+1,-1)),

			Index(index + Index(-1,0)),
			Index(index + Index(+1,0)),

			Index(index + Index(-1,+1)),
			Index(index + Index(+0, +1)),
			Index(index + Index(+1,+1))
		};

		for (int i = 0; i < 8; i++)
		{
			AICostMap[surroundingIndexsLayer1[i]] += 2;
		}
	}
}


#if DRAW_AI_PATH_COSTMAP
void AIController::drawCostMap()
{
	int ptSize = 16;
	RenderColour colour = RenderColour::Red;

	char text[3];
	Camera* camera = Camera::Get();

	for (int y = 0; y < mPathMap.yCount(); y++)
	{
		for (int x = 0; x < mPathMap.xCount(); x++)
		{
			Index index(x, y);
			VectorF position = mPathMap[index].rect().TopLeft();

			if (camera->inView(position) && mPathMap[index].has(CollisionTile::Floor))
			{
				_itoa_s(mPathMap.costMap().get(index), text, 10);
				debugRenderText(text, ptSize, position, colour);
			}
		}
	}
}
#endif