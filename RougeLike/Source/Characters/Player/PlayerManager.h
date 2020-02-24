#pragma once

#include "Collisions/CollisionTracker.h"
#include "Events/Dispatcher.h"
#include "Events/Observer.h"

struct GameData;
class Player;
class Collider;
class EnemyManager;

class PlayerManager : public Dispatcher, public Observer
{
public:
	PlayerManager(GameData* gameData);
	~PlayerManager();

	Player* get() { return player; }
	RectF* getRectRef();

	std::vector<Collider*> getWeaponColliders();

	void handleEvent(const Event event, EventData& data) override;
	void handleEvent(const EventPacket) override { }

	void init(EnemyManager* enemyManager);
	void preProcess();
	void handleInput();
	void slowUpdate(float dt);
	void fastUpdate(float dt);

	// testing
	bool gotHit = false;

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