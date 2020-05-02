#pragma once

#include "Events/Dispatcher.h"
#include "Events/Observer.h"

#include "Weapons/WeaponStash.h"
#include "Collisions/CollisionTracker.h"
#include "Objects/Attributes/StatManager.h"

#include "Objects/Effects/EffectHandler.h"

struct GameData;
class Player;
class Map;


// TEMP
class Effect;

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
	RectF* rect();
	VectorF position() const;

	void handleEvent(const Event event, EventData& data) override;

private:
	void processHit();

	void resolveWallCollisions	(const Map* map, float dt);
	bool doesCollideLeft		(const Map* map, const VectorF point, float dt) const;
	bool doesCollideRight		(const Map* map, const VectorF point, float dt) const;
	bool doesCollideTop			(const Map* map, const VectorF point, float dt) const;
	bool doesCollideBot			(const Map* map, const VectorF point, float dt) const;

	void updateUIStats();


private:
	GameData* mGameData;

	Player* player;

	StatManager statManager;
	WeaponStash weaponStash;

	Vector2D<int> tileIndex;

	EffectHandler mEffectHandler;
};