#include "pch.h"
#include "TrapManager.h"

#include "Map.h"

#include "Collisions/CollisionTracker.h"

void TrapManager::init(Map* map)
{
	mMap = map;

	for (int y = 0; y < map->yCount(); y++)
	{
		for (int x = 0; x < map->xCount(); x++)
		{
			Index index(x, y);
			MapTile& tile = mMap->get(index);

			if (tile.has(DecorTile::Spikes))
			{
				EffectCollider collider(&tile.rectRef());

				// add damage effect  

				mColliders.push_back(collider);
			}
		}
	}

}

void TrapManager::addTrapColliders(Map* map, CollisionTracker* tracker)
{
	mMap = map;

	

	tracker->addDefenders();
}


void TrapManager::slowUpdate(Map* map)
{
	updateTriggerTraps(map);
	updateResetTraps(map);
}

void TrapManager::triggerTrap(Map* map, VectorF position)
{
	Index index = map->index(position);
	MapTile& tile = map->get(index);

	// Add untriggered trap
	if (tile.has(DecorTile::Spikes))
	{
		Animator& animator = tile.animation(0);

		if (animator.animation().currentFrame() == 0)
		{
			IndexTimer indexTimer(index, TimerF(TimerF::Start));
			mUntriggeredTraps.push(indexTimer);
		}
	}
}


void TrapManager::updateTriggerTraps(Map* map)
{
	if (mUntriggeredTraps.size() > 0 && mUntriggeredTraps.front().timer.getSeconds() > 1.0f)
	{
		Index trapIndex = mUntriggeredTraps.front().index;
		MapTile& tile = map->get(trapIndex);
		Animator& animator = tile.animation(0);

		// TODO: this is being triggered everyframe, make it only once
		if (animator.animation().currentFrame() == 0)
		{
			animator.getAnimation(0).nextFrame();

			IndexTimer& trap = mUntriggeredTraps.popFront();
			trap.timer.restart();

			mTriggeredTraps.push(trap);
		}
	}
}

void TrapManager::updateResetTraps(Map* map)
{
	if (mTriggeredTraps.size() > 0 && mTriggeredTraps.front().timer.getSeconds() > 2.0f)
	{
		Index trapIndex = mTriggeredTraps.front().index;
		MapTile& tile = map->get(trapIndex);
		Animator& animator = tile.animation(0);

		if (animator.animation().currentFrame() == 1)
		{
			animator.getAnimation(0).nextFrame();
			mTriggeredTraps.pop();
		}
	}
}