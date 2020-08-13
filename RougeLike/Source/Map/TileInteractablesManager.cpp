#include "pch.h"
#include "TileInteractablesManager.h"

#include "Map.h"
#include "TileInteractables/SpikeTrap.h"
#include "TileInteractables/FireGratingTrap.h"
#include "TileInteractables/FireGratingTrigger.h"

#include "Collisions/CollisionManager.h"


void TileInteractableManager::load()
{
	std::vector<DecorType> types;
	for (DecorType type = DecorType::Traps << 1; type < DecorType::END; type = type << 1)
	{
		types.push_back(type);
	}
	mTrapPool.load(types, 50);
}


void TileInteractableManager::addMap(Map* map, const DecorMap& info)
{
	//if (mInteractables.size() > 0)
	//	return;
	Interactables interactables;
	interactables.init();

	for (int y = 0; y < map->yCount(); y++)
	{
		for (int x = 0; x < map->xCount(); x++)
		{
			Index index(x, y);
			MapTile* tile = map->tile(index);

			if (tile->decorType() > DecorType::Triggers)
			{
				Trigger* trigger = mTrapPool.getTrigger(tile->decorType());
				prepare(trigger, tile, info);
				trigger->linkTraps(interactables.traps);
				interactables.triggers->push_back(trigger);

			}
			else if (tile->decorType() > DecorType::Traps)
			{
				Trap* trap = mTrapPool.getTrap(tile->decorType());
				prepare(trap, tile, info);
				trap->fillData(info);
				interactables.traps->push_back(trap);
			}
		}
	}

	mInteractables.push_back(interactables);
}


void TileInteractableManager::popFrontMap()
{
 	Interactables interactables = mInteractables.front();
	mInteractables.erase(mInteractables.begin());

	for (Trigger* tileTigger : *interactables.triggers)
	{
		mCollisions->removeDefender(CollisionManager::Player_Trigger_Trap, tileTigger->collider());
		mTrapPool.returnObject(tileTigger, tileTigger->type());
		tileTigger->clear();
	}

	for (Trap* trap : *interactables.traps)
	{
		mCollisions->removeDefender(CollisionManager::Player_Trigger_Trap, trap->collider());
		mCollisions->removeAttacker(CollisionManager::Trap_Hit_Player, trap->collider());
		mTrapPool.returnObject(trap, trap->type());
		trap->clear();
	}

	interactables.free();
}



void TileInteractableManager::prepare(TileInteractable* interactable, MapTile* tile, const DecorMap& info)
{
	interactable->init(tile);
	mCollisions->addDefender(CollisionManager::Player_Trigger_Trap, interactable->collider());
}


void TileInteractableManager::slowUpdate()
{
	for (int i = 0; i < mInteractables.size(); i++)
	{
		for (Trigger* tileTigger : *mInteractables[i].triggers)
		{
			if (tileTigger->canTrigger())
			{
				tileTigger->trigger();
				mCollisions->removeDefender(CollisionManager::Player_Trigger_Trap, tileTigger->collider());
			}
		}

		for (Trap* trap : *mInteractables[i].traps)
		{
			trap->update(mEffects);

			if (trap->gotHit())
				set(trap);

			if (trap->canTrigger())
				trigger(trap);

			if (trap->canReset())
				reset(trap);
		}
	}
}


void TileInteractableManager::set(Trap* trap)
{
	mCollisions->removeDefender(CollisionManager::Player_Trigger_Trap, trap->collider());
	trap->set();
}


void TileInteractableManager::trigger(TileInteractable* interactable)
{
	interactable->trigger();
	mCollisions->addAttacker(CollisionManager::Trap_Hit_Player, interactable->collider());
}


void TileInteractableManager::reset(Trap* trap)
{
	trap->reset();
	mCollisions->removeAttacker(CollisionManager::Trap_Hit_Player, trap->collider());
	mCollisions->addDefender(CollisionManager::Player_Trigger_Trap, trap->collider());
}
