#pragma once

class EnemyManager;
class CollisionManager;
class Collider;
class Enemy;

class EnemyCollisions
{
public:
	EnemyCollisions();

	void init(CollisionManager* collisions) { mCollisionManager = collisions; }
	void clear() { }

	void updateAttackingColliders(std::vector<Collider*> attackingColliders);
	void add(Collider* collider);
	void remove(Collider* collider);

#if _DEBUG
	bool isEmpty();
#endif

private:
	CollisionManager* mCollisionManager;

};