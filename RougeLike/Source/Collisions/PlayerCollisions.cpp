#include "pch.h"
#include "PlayerCollisions.h"

#include "Actors/Player/Player.h"
#include "Weapons/Melee/MeleeWeapon.h"
#include "CollisionManager.h"


void PlayerCollisions::init(Player* player, CollisionManager* collisionManager)
{
	mPlayer = player;
	cManager = collisionManager;

	mWallCollisions.setActor(mPlayer);
	mWeaponCollisions.init(player, collisionManager->getTracker(CollisionManager::PlayerWeapon_Hit_Enemy));

	addCollidersToTrackers();
	enableCollisions(CollisionManager::Player_Hit_Collectable, true);
	enableCollisions(CollisionManager::PlayerWeapon_Hit_Enemy, false);
}

void PlayerCollisions::clear()
{
	mPlayer = nullptr;
	cManager = nullptr;
	mWeaponCollisions.clear();
}

void PlayerCollisions::resolveWalls(Map* map, float dt)
{
	if (mPlayer->userHasControl())
	{
#if !IGNORE_WALLS
		VectorF movement = mPlayer->physics()->movementDistance(dt);
		movement = mWallCollisions.allowedMovement(map, movement);
		mPlayer->physics()->setVelocity(movement / dt);

		mPlayer->physics()->move(dt);
#endif
	}
}


void PlayerCollisions::resolveWeapons(EffectPool* effects)
{
	if (mPlayer->mWeapon->isAttacking())
	{
		enableCollisions(CollisionManager::PlayerWeapon_Hit_Enemy, true);
		mWeaponCollisions.processWeaponEffects(effects);
		mWeaponCollisions.addCollidersToExcludedList();
	}
	else
	{
		enableCollisions(CollisionManager::PlayerWeapon_Hit_Enemy, false);
	}
}


void PlayerCollisions::refreshWeaponColliders()
{
	cManager->removeAllAttackers(CollisionManager::PlayerWeapon_Hit_Enemy);
	cManager->addAttackers(CollisionManager::PlayerWeapon_Hit_Enemy, mPlayer->weapon()->getColliders());
}

void PlayerCollisions::clearExcludedColliders(CollisionManager::Tracker tracker)
{
	CollisionTracker* collisionTracker = cManager->getTracker(tracker);
	if (collisionTracker)
		collisionTracker->clearExcludedDefenders();
}


void PlayerCollisions::enableCollisions(CollisionManager::Tracker tracker, bool isEnabled)
{
	CollisionTracker* collisionTracker = cManager->getTracker(tracker);
	if (collisionTracker)
		collisionTracker->setCheckingStatus(isEnabled);
}


// -- Private Functions -- //

void PlayerCollisions::addCollidersToTrackers()
{
	std::vector<Collider*> playerCollider{ mPlayer->collider() };
	cManager->addDefenders(CollisionManager::Enemy_Hit_Player, playerCollider);
	cManager->addDefenders(CollisionManager::Trap_Hit_Player, playerCollider);
	cManager->addAttackers(CollisionManager::Player_Trigger_Trap, playerCollider);
	cManager->addAttackers(CollisionManager::Player_Hit_Collectable, playerCollider);
}
