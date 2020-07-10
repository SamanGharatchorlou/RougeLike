#include "pch.h"
#include "PlayerCollisions.h"


#include "Game/GameData.h"
#include "Objects/Effects/EffectPool.h"
#include "Objects/Effects/DamageEffect.h"

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
	cManager->addAttackers(CollisionManager::Player_Hit_Trap, playerCollider);
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
	map->triggerTraps(mPlayer->position());
}


void PlayerCollisions::resolveTrapCollisions(const Map* map)
{
	Effect* effect = mPlayer->mGameData->effectPool->getEffect(EffectType::Damage);
	DamageEffect* damageEffect = static_cast<DamageEffect*>(effect);
	damageEffect->set(Damage(100));

	EffectCollider trapCollider;
	trapCollider.addEffect(damageEffect);

	


	//const MapTile* tile = map->tile(mPlayer->position());

	//// Add untriggered trap
	//if (tile->has(DecorTile::Spikes))
	//{
	//	const Animator& animator = tile->animation(0);

	//	if (animator.animation().currentFrame() == 1)
	//	{
	//		DamageEffect damage
	//	}
	//}
}

/*
void PlayerCollisions::updateTraps(Map* map)
{
	// Trigger trap
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

	// Reset trap
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


void PlayerCollisions::resolveTrapCollisions(const Map* map)
{
	const MapTile* tile = map->tile(mPlayer->position());

	// Add untriggered trap
	if (tile->has(DecorTile::Spikes))
	{
		const Animator& animator = tile->animation(0);

		if (animator.animation().currentFrame() == 1)
		{
			DamageEffect damage
		}
	}
}
*/