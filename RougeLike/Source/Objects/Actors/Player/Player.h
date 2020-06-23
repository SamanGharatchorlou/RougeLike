#pragma once

#include "Objects/Actors/Actor.h"

#include "Weapons/WeaponStash.h"
#include "Collisions/PlayerCollisions.h"
#include "Objects/Abilities/AbilityManager.h"


struct GameData;
class Weapon;
class MeleeWeapon;


class Player : public Actor
{
public:
	Player(GameData* gameData);
	~Player() { };

	void init(const std::string& characterConfig);
	void handleInput();
	void fastUpdate(float dt);
	void effectLoop() override;
	void slowUpdate(float dt);
	void render();

	void reset();

	void loadWeaponStash();
	Weapon*	weapon();

	void selectCharacter(const std::string& character);
	void selectWeapon(const std::string& weaponName);

	void addAbility(const std::string& name);

	void userHasControl(bool removeControl);

	void updateUI();

	void enableBodyCollisions(bool isEnabled);


private:
	void updateProperties();

	void processHit();

	void attack();
	void updateWeaponHitSound();

	void updateCurrentTile();


private:
	PlayerCollisions mCollisionManager;

	AbilityManager mAbilities;

	//StatManager mStatManager;
	WeaponStash weaponStash;

	Vector2D<int> tileIndex;

	MeleeWeapon* mWeapon;

	bool mControlOverride;
};