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
	//mCollisionManager->addDefenders(CollisionManager::Player_Hit_Enemy, defendingCollider);

	mCollisionManager->addDefenders(CollisionManager::Effect_Hit_Enemy, defendingCollider);
}

void EnemyCollisions::remove(Collider* collider)
{
	mCollisionManager->removeDefender(CollisionManager::PlayerWeapon_Hit_Enemy, collider);
	//mCollisionManager->removeDefender(CollisionManager::Player_Hit_Enemy, collider);


	mCollisionManager->removeDefender(CollisionManager::Effect_Hit_Enemy, collider);
}

#if _DEBUG
bool EnemyCollisions::isEmpty()
{
	int countA = mCollisionManager->defenderCount(CollisionManager::PlayerWeapon_Hit_Enemy);
	//int countB = mCollisionManager->defenderCount(CollisionManager::Player_Hit_Enemy);


	int countB = mCollisionManager->defenderCount(CollisionManager::Effect_Hit_Enemy);

	return countA + countB == 0;
}
#endif