#pragma once

#include "Events/Dispatcher.h"

#include "Collisions/CollisionTracker.h"
#include "EnemyEnums.h"


struct GameData;
class Enemy;


class EnemyManager : public Dispatcher
{
public:
	enum ObjectStatus
	{
		None,
		Uninitialised,
		Available,
		Active,
		Inactive,
	};

	using EnemyObject = std::pair<Enemy*, ObjectStatus>;

public:
	EnemyManager(GameData* gameData);
	~EnemyManager();

	void slowUpdate(float dt);
	void fastUpdate(float dt);
	void render() const;

	void clearAttackingColliders();
	void addAttackingColliders(std::vector<Collider*> colliders);

	void addEnemiesToPool(EnemyType type, unsigned int count);

	void spawn(EnemyType type, unsigned int xPositionPercentage);

	void setTarget(RectF* rect) { mTarget = rect; }

	Enemy* getEnemy(unsigned int index) const;

	std::vector<Collider*> getAttackingColliders() const;

	unsigned int size() const { return mActiveEnemies.size(); }



private:
	void deactivate(std::vector<Enemy*>::iterator& iter);


private:
	GameData* mGameData;

	std::vector<EnemyObject> mEnemyPool;
	std::vector<Enemy*> mActiveEnemies;

	// Player weapon <-> enemy
	CollisionTracker mCollisionManager;

	RectF* mTarget;
};