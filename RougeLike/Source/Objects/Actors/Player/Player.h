#pragma once

#include "Objects/Actors/Actor.h"
#include "States/State.h"
#include "States/StateMachine.h"

#include "Weapons/WeaponStash.h"
#include "Objects/Attributes/StatManager.h"

#include "Collisions/WallCollisionTracker.h"

#include "Objects/Abilities/AbilityManager.h"


struct GameData;
class Map;
class Weapon;
class MeleeWeapon;


class Player : public Actor
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

	void initCollisions();

	void loadWeaponStash();
	Weapon*	weapon();

	void selectCharacter(const std::string& character);
	void selectWeapon(const std::string& weaponName);

	void addAbility(Ability* ability);

	StatManager& statManager() { return mStatManager; }

	void userHasControl(bool removeControl);


private:
	void initPropertBag(XMLParser& parser);

	void processHit();

	void attack();
	void updateAttackingWeapon();

	void updateCurrentTile();

private:

	StateMachine<State> mStateMachine;
	WallCollisionTracker mWallCollisions;

	AbilityManager mAbilities;

	StatManager mStatManager;
	WeaponStash weaponStash;

	Vector2D<int> tileIndex;

	MeleeWeapon* mWeapon;

	bool mControlOverride;
};