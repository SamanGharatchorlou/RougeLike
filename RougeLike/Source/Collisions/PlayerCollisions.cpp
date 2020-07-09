#include "pch.h"
#include "PlayerCollisions.h"

#include "Objects/Actors/Player/Player.h"
#include "Weapons/Weapon.h"
#include "CollisionManager.h"

#include "Map/Map.h"


PlayerCollisions::PlayerCollisions(Player* player, CollisionManager* collisionManager) 
	: mPlayer(player), cManager(collisionManager), mWallCollisions(player) { }


void PlayerCollisions::addColliderToTrackers()
{
	std::vector<Collider*> playerCollider{ mPlayer->collider() };
	cManager->addDefenders(CollisionManager::Enemy_Hit_Player, playerCollider);
	cManager->addAttackers(CollisionManager::Player_Hit_Collectable, playerCollider);
	cManager->addAttackers(CollisionManager::Player_Hit_Enemy, playerCollider);
}


void PlayerCollisions::resolveWallCollisions(const Map* map, float dt)
{
	mWallCollisions.resolveWallCollisions(map, dt);
}


void PlayerCollisions::updateWeaponColliders()
{
	if (mPlayer->weapon()->isAttacking())
	{
		addEnemiesToExcludedList();
	}
	else
	{
		enableWeaponCollisions(false);
	}
}


void PlayerCollisions::refreshWeaponColliders()
{
	cManager->removeAllAttackers(CollisionManager::PlayerWeapon_Hit_Enemy);
	cManager->addAttackers(CollisionManager::PlayerWeapon_Hit_Enemy, mPlayer->weapon()->getColliders());
}


void PlayerCollisions::clearExcludedList()
{
	CollisionTracker* tracker = cManager->getTracker(CollisionManager::PlayerWeapon_Hit_Enemy);
	ComplexCollisionTracker* complexTracker = static_cast<ComplexCollisionTracker*>(tracker);
	complexTracker->clearExcludedDefenders();
}


void PlayerCollisions::addEnemiesToExcludedList()
{
	CollisionTracker* simpleTracker = cManager->getTracker(CollisionManager::PlayerWeapon_Hit_Enemy);
	ComplexCollisionTracker* complexTracker = static_cast<ComplexCollisionTracker*>(simpleTracker);

	std::vector<Collider*> enemies = complexTracker->defenders();

	for (int i = 0; i < enemies.size(); i++)
	{
		if (enemies[i]->gotHit())
		{
			complexTracker->addExcludedDefender(enemies[i]);
		}
	}
}


void PlayerCollisions::enableWeaponCollisions(bool isEnabled)
{
	CollisionTracker* simpleTracker = cManager->getTracker(CollisionManager::PlayerWeapon_Hit_Enemy);
	ComplexCollisionTracker* complexTracker = static_cast<ComplexCollisionTracker*>(simpleTracker);
	complexTracker->setCheckingStatus(isEnabled);
}


void PlayerCollisions::enableBodyCollisions(bool isEnabled)
{
	CollisionTracker* simpleTracker = cManager->getTracker(CollisionManager::Player_Hit_Enemy);
	ComplexCollisionTracker* complexTracker = static_cast<ComplexCollisionTracker*>(simpleTracker);
	complexTracker->setCheckingStatus(isEnabled);
}

void PlayerCollisions::triggerTraps(Map* map)
{
	Index index = map->index(mPlayer->position());
	MapTile& tile = map->get(index);// map->tile(mPlayer->position());

	// Add untriggered trap
	if (tile.has(DecorTile::Spikes))
	{
		Animator& animator = tile.animation(0);

		if (animator.animation().currentFrame() == 0)
		{
			std::pair<Index, TimerF> indexTimer(index, TimerF(TimerF::Start));
			mUntriggeredTraps.push(indexTimer);
		}
	}
}

void PlayerCollisions::updateTraps(Map* map)
{
	// Trigger trap
	if (mUntriggeredTraps.size() > 0 && mUntriggeredTraps.front().second.getSeconds() > 1.0f)
	{
		Index trapIndex = mUntriggeredTraps.front().first;
		MapTile& tile = map->get(trapIndex);

		Animator& animator = tile.animation(0);

		// TODO: this is being triggered everyframe, make it only once
		if (animator.animation().currentFrame() == 0)
		{
			animator.getAnimation(0).nextFrame();

			std::pair<Index, TimerF>& trap = mUntriggeredTraps.front();
			trap.second.restart();

			mUntriggeredTraps.pop();
			mTriggeredTraps.push(trap);
		}
	}

	// Reset trap
	if (mTriggeredTraps.size() > 0 && mTriggeredTraps.front().second.getSeconds() > 2.0f)
	{
		Index trapIndex = mTriggeredTraps.front().first;
		MapTile& tile = map->get(trapIndex);

		Animator& animator = tile.animation(0);

		if (animator.animation().currentFrame() == 1)
		{
			animator.getAnimation(0).nextFrame();
			mTriggeredTraps.pop();
		}
	}
}