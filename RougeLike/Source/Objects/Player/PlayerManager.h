#pragma once

#include "Events/Dispatcher.h"
#include "Events/Observer.h"

#include "Objects/Actor.h"
#include "States/StateMachine.h"

#include "Weapons/WeaponStash.h"
#include "Objects/Attributes/StatManager.h"

#include "Objects/Effects/EffectHandler.h"

struct GameData;
class Map;
class Weapon;
class PlayerPropertyBag;


class PlayerManager : public Dispatcher, public Observer, public Actor
{
public:
	PlayerManager(GameData* gameData);
	~PlayerManager();

	void init(const std::string& characterConfig);
	void handleInput();
	void slowUpdate(float dt);
	void fastUpdate(float dt);
	void render();

	void reset();

	void initCollisions();

	void loadWeaponStash();
	Weapon*		weapon() { return mWeapon; }

	void selectCharacter(const std::string& character);
	void selectWeapon(const std::string& weaponName);

	void handleEvent(const Event event, EventData& data) override;


private:
	void processHit();

	void resolveWallCollisions	(const Map* map, float dt);
	bool doesCollideLeft		(const Map* map, const VectorF point, float dt) const;
	bool doesCollideRight		(const Map* map, const VectorF point, float dt) const;
	bool doesCollideTop			(const Map* map, const VectorF point, float dt) const;
	bool doesCollideBot			(const Map* map, const VectorF point, float dt) const;

	void updateUIStats();

	RectF renderRect() const;

private:

	StateMachine<State> mStateMachine;

	StatManager statManager;
	WeaponStash weaponStash;

	Vector2D<int> tileIndex;

	EffectHandler mEffectHandler;

	Weapon* mWeapon;

	bool mMoving;
};