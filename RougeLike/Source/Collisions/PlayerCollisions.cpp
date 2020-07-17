#include "pch.h"
#include "PlayerCollisions.h"

#include "Game/GameData.h"
#include "Objects/Effects/EffectTypes/DamageEffect.h"
#include "Objects/Effects/EffectTypes/DisplacementEffect.h"

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

void PlayerCollisions::fastUpdate(float dt, Map* map)
{
	if (mPlayer->userHasControl())
	{
#if !IGNORE_WALLS
		mWallCollisions.resolveWallCollisions(map, dt);
#endif
	}
}

void PlayerCollisions::slowUpdate(Map* map)
{
	map->traps().triggerTrap(mPlayer->position());
	resolveTrapCollisions(map);

	updateWeaponColliders();

	// TODO: not set this every frame... feels dirty
	enableCollisions(CollisionManager::Player_Hit_Enemy, mPlayer->hasBodyCollisions());
}


// Weapon collisions
void PlayerCollisions::updateWeaponColliders()
{
	if (mPlayer->mWeapon->isAttacking())
	{
		enableCollisions(CollisionManager::PlayerWeapon_Hit_Enemy, true);
		mWeaponCollisions.processWeaponEffects();
		mWeaponCollisions.addEnemiesToExcludedList();
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
	cManager->addAttackers(CollisionManager::Player_Hit_Collectable, playerCollider);
	cManager->addAttackers(CollisionManager::Player_Hit_Enemy, playerCollider);
}


void PlayerCollisions::resolveTrapCollisions(Map* map)
{
	if (map->traps().didCollide(mPlayer->position()))
	{
		Effect* effect = mPlayer->getEffectFromPool(EffectType::Damage);
		DamageEffect* damageEffect = static_cast<DamageEffect*>(effect);
		damageEffect->set(Damage(10));
		mPlayer->addEffect(damageEffect);
	}
}