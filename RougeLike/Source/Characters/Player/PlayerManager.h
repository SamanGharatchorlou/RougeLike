#pragma once

#include "Events/Dispatcher.h"
#include "Events/Observer.h"

#include "Weapons/WeaponStash.h"
#include "Collisions/CollisionTracker.h"
#include "Characters/Attributes/StatManager.h"

struct GameData;
class Player;
class Collider; // TODO: do I need this?


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
	void render();

	void selectCharacter(const std::string& character);
	void selectWeapon(const std::string& weaponName);

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

	void updateUIStats();


private:
	GameData* mGameData;

	Player* player;

	StatManager statManager;

	WeaponStash weaponStash;

	CollisionTracker collisionTracker;
};