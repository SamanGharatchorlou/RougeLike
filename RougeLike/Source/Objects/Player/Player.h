#pragma once

#include "Events/Dispatcher.h"
#include "Events/Observer.h"

#include "Objects/Actor.h"
#include "States/State.h"
#include "States/StateMachine.h"

#include "Weapons/WeaponStash.h"
#include "Objects/Attributes/StatManager.h"


struct GameData;
class Map;
class Weapon;


class Player : public Dispatcher, public Observer, public Actor
{
public:
	Player(GameData* gameData);
	~Player() { };

	void init(const std::string& characterConfig);
	void handleInput();
	void slowUpdate(float dt);
	void fastUpdate(float dt);
	void render();

	void reset();

	void handleEvent(const Event event, EventData& data) override;

	void initCollisions();

	void loadWeaponStash();
	Weapon*		weapon() { return mWeapon; }

	void selectCharacter(const std::string& character);
	void selectWeapon(const std::string& weaponName);



private:
	void processHit();

	void resolveWallCollisions	(const Map* map, float dt);
	bool doesCollideLeft		(const Map* map, const VectorF point, float dt) const;
	bool doesCollideRight		(const Map* map, const VectorF point, float dt) const;
	bool doesCollideTop			(const Map* map, const VectorF point, float dt) const;
	bool doesCollideBot			(const Map* map, const VectorF point, float dt) const;

	RectF renderRect() const override;

private:

	StateMachine<State> mStateMachine;

	StatManager statManager;
	WeaponStash weaponStash;

	Vector2D<int> tileIndex;

	Weapon* mWeapon;

	bool mMoving;
};