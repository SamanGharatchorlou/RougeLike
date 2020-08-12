#include "pch.h"
#include "TrapManager.h"

#include "Map.h"
#include "Traps/Trap.h"

#include "Collisions/CollisionManager.h"


void TrapManager::addTraps(Map* map, const DecorMap& info)
{
	for (int y = 0; y < map->yCount(); y++)
	{
		for (int x = 0; x < map->xCount(); x++)
		{
			Index index(x, y);
			MapTile* tile = map->tile(index);

			if (tile->has(DecorType::Spikes))
			{
				SpikeTrap* trap = new SpikeTrap(tile);
				trap->fillData(info);
				mTraps.push_back(trap);

				mCollisions->addDefender(CollisionManager::Player_Trigger_Trap, trap->collider());
			}

			//if (tile->has(DecorType::Grating))
			//{
			//	FireGratingTrap* trap = new FireGratingTrap(tile);
			//	trap->fillData(info);
			//	mTraps.push_back(trap);

			//	mCollisions->addDefender(CollisionManager::Player_Trigger_Trap, trap->collider());
			//}
		}
	}
}


void TrapManager::slowUpdate()
{
	setTraps();
	triggerTraps();
	resetTraps();
}


void TrapManager::setTraps()
{
	for (std::vector<Trap*>::iterator iter = mTraps.begin(); iter != mTraps.end(); iter++)
	{
		Trap* trap = *iter;
		if (trap->gotHit())
		{
			mCollisions->removeDefender(CollisionManager::Player_Trigger_Trap, trap->collider());
			trap->set();
			trap->init(mEffects);
			break;
		}
	}
}


void TrapManager::triggerTraps()
{
	for (std::vector<Trap*>::iterator iter = mTraps.begin(); iter != mTraps.end(); iter++)
	{
		Trap* trap = *iter;
		if (trap->canTrigger())
		{
			trap->trigger();
			mCollisions->addAttacker(CollisionManager::Trap_Hit_Player, trap->collider());
			break;
		}
	}
}


void TrapManager::resetTraps()
{
	for (std::vector<Trap*>::iterator iter = mTraps.begin(); iter != mTraps.end(); iter++)
	{
		Trap* trap = *iter;
		if (trap->canReset())
		{
			trap->reset();

			mCollisions->removeAttacker(CollisionManager::Trap_Hit_Player, trap->collider());
			mCollisions->addDefender(CollisionManager::Player_Trigger_Trap, trap->collider());
			break;
		}
	}
}
