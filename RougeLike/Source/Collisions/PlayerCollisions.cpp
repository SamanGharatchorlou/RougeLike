#include "pch.h"
#include "PlayerCollisions.h"

#include "Actors/Player/Player.h"
#include "Weapons/Melee/MeleeWeapon.h"
#include "CollisionManager.h"


PlayerCollisions::~PlayerCollisions()
{
	clear();
}


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
		VectorF velocity = mPlayer->physics()->velocity();
		velocity = mWallCollisions.allowedVelocity(map, velocity, dt);
		mPlayer->physics()->setVelocity(velocity);
#endif
		mPlayer->physics()->move(dt);
	}
}


void PlayerCollisions::resolveWeapons()
{
	if (mPlayer->mWeapon->isAttacking())
	{
		enableCollisions(CollisionManager::PlayerWeapon_Hit_Enemy, true);
		mWeaponCollisions.processWeaponEffects();
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
	cManager->addAttacker(CollisionManager::PlayerWeapon_Hit_Enemy, mPlayer->weapon()->getCollider());
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
