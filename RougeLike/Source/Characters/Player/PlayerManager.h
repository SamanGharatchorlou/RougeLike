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
	bool doesCollideLeft(const VectorF point, float dt) const;
	bool doesCollideRight(const VectorF point, float dt) const;
	bool doesCollideTop(const VectorF point, float dt) const;
	bool doesCollideBot(const VectorF point, float dt) const;

	void updateTrackedColliders();

private:
	GameData* mGameData;
	EnemyManager* mEnemyManager;

	Player* player;

	CollisionTracker collisionTracker;
};