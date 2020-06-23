#include "pch.h"
#include "PlayerCollisions.h"

#include "Objects/Actors/Player/Player.h"
#include "Weapons/Weapon.h"
#include "CollisionManager.h"


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
