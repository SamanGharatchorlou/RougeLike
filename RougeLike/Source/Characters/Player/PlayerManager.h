#pragma once

#include "Collisions/CollisionTracker.h"

struct GameData;
class Player;
class Collider;
class EnemyManager;

class PlayerManager
{
public:
	PlayerManager(GameData* gameData, EnemyManager* enemyManager);

	Player* get() { return player; }
	RectF* getRectRef();

	std::vector<Collider*> getWeaponColliders();

	void init();
	void preProcess();
	void handleInput();
	void slowUpdate(float dt);
	void fastUpdate(float dt);


private:
	void resolveWallCollisions(float dt);
	bool doesCollideLeft(VectorF point, float dt);
	bool doesCollideRight(VectorF point, float dt);
	bool doesCollideTop(VectorF point, float dt);
	bool doesCollideBot(VectorF point, float dt);

	void updateTrackedColliders();

private:
	GameData* mGameData;
	EnemyManager* mEnemyManager;

	Player* player;

	CollisionTracker collisionTracker;
};