#include "pch.h"
#include "PlayerCollisions.h"

#include "Objects/Effects/EffectPool.h"
#include "Objects/Effects/EffectTypes/Effect.h"

#include "Actors/Player/Player.h"
#include "Weapons/Melee/MeleeWeapon.h"
#include "CollisionManager.h"

#include "Map/Map.h"
#include "Map/TrapManager.h"


void PlayerCollisions::init(Player* player, CollisionManager* collisionManager)
{
	mPlayer = player;
	cManager = collisionManager;

	mWallCollisions.init(player);
	mWeaponCollisions.init(player, collisionManager->getTracker(CollisionManager::PlayerWeapon_Hit_Enemy));

	addCollidersToTrackers();
	enableCollisions(CollisionManager::Player_Hit_Collectable, true);
	enableCollisions(CollisionManager::Player_Hit_Enemy, false);
	enableCollisions(CollisionManager::PlayerWeapon_Hit_Enemy, false);
}

void PlayerCollisions::resolveWalls(Map* map, float dt)
{
	if (mPlayer->userHasControl())
	{
#if !IGNORE_WALLS
		mWallCollisions.resolveWallCollisions(map, dt);
#endif
	}
}

void PlayerCollisions::resolveTraps(Map* map, EffectPool* effects)
{
	map->traps().triggerTrap(mPlayer->position());
	resolveTrapCollisions(map, effects);
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

void PlayerCollisions::enableBodyCollisions(bool enable)
{
	// TODO: not set this every frame... feels dirty
	enableCollisions(CollisionManager::Player_Hit_Enemy, enable);
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
	cManager->addAttackers(CollisionManager::Player_Hit_Collectable, playerCollider);
	cManager->addAttackers(CollisionManager::Player_Hit_Enemy, playerCollider);
}


void PlayerCollisions::resolveTrapCollisions(Map* map, EffectPool* effects)
{
	if (map->traps().didCollide(mPlayer->position()))
	{
		Damage trapDamage = map->traps().damage();

		FloatMap valueMap;
		valueMap["Damage"] = trapDamage.value();
		PropertyBag properties(valueMap);

		Effect* effect = effects->getObject(EffectType::Damage);
		effect->fill(properties);
		mPlayer->addEffect(effect);
	}
}