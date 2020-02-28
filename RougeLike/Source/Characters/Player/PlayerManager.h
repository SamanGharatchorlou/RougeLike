#pragma once

#include "Events/Dispatcher.h"
#include "Events/Observer.h"

#include "Collisions/CollisionTracker.h"
#include "Characters/Weapons/WeaponStash.h"
#include "Characters/Attributes/StatManager.h"

struct GameData;
class Player;
class Collider;


class PlayerManager : public Dispatcher, public Observer
{
public:
	PlayerManager(GameData* gameData);
	~PlayerManager();

	void init();
	void preProcess();
	void handleInput();
	void slowUpdate(float dt);
	void fastUpdate(float dt);

	void selectCharacter(const std::string& character);
	void selectWeapon(const std::string& weapon);

	Player* get() { return player; }
	RectF* getRectRef();

	std::vector<Collider*> getWeaponColliders();

	void handleEvent(const Event event, EventData& data) override;

private:
	void resolveWallCollisions(float dt);
	bool doesCollideLeft(const VectorF point, float dt) const;
	bool doesCollideRight(const VectorF point, float dt) const;
	bool doesCollideTop(const VectorF point, float dt) const;
	bool doesCollideBot(const VectorF point, float dt) const;

	void updateTrackedColliders();


private:
	GameData* mGameData;

	Player* player;

	StatManager statManager;

	WeaponStash weaponStash;

	CollisionTracker collisionTracker;
};