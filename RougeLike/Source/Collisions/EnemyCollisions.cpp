#include "pch.h"
#include "EnemyCollisions.h"

#include "CollisionManager.h"


EnemyCollisions::EnemyCollisions() : mCollisionManager(nullptr) { }


void EnemyCollisions::updateAttackingColliders(std::vector<Collider*> attackingColliders)
{
	mCollisionManager->removeAllAttackers(CollisionManager::Enemy_Hit_Player);
	mCollisionManager->addAttackers(CollisionManager::Enemy_Hit_Player, attackingColliders);
}

void EnemyCollisions::add(Collider* collider)
{
	std::vector<Collider*> defendingCollider{ collider };

	mCollisionManager->addDefenders(CollisionManager::PlayerWeapon_Hit_Enemy, defendingCollider);
}

void EnemyCollisions::remove(Collider* collider)
{
	mCollisionManager->removeDefender(CollisionManager::PlayerWeapon_Hit_Enemy, collider);
}

#if _DEBUG
bool EnemyCollisions::isEmpty()
{
	int count = mCollisionManager->defenderCount(CollisionManager::PlayerWeapon_Hit_Enemy);;

	return count == 0;
}
#endif