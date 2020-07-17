#pragma once


#include "Player.h"

#include "Collisions/PlayerCollisions.h"
#include "Weapons/WeaponStash.h"
#include "Events/LocalDispatcher.h"

#include "Objects/Abilities/AbilityManager.h"


struct GameData;
struct Environment;

class PlayerManager
{
public:
	PlayerManager(GameData* gameData);

	Player* get() { return &mPlayer; }

	void init(Environment* environment);

	void handleInput(const InputManager* input);
	void fastUpdate(float dt);
	void slowUpdate(float dt);
	void render();
	void exit();


	LocalDispatcher& events() { return mEvents; }

	void setPosition(VectorF position);
	void selectCharacter(const std::string& characterConfig) { mPlayer.init(characterConfig); }
	void selectWeapon(const std::string& weaponName);


private:
	Environment* mEnvironment;

	Player mPlayer;
	AbilityManager mAbilities;

	// Player stuff
	PlayerCollisions mPlayerCollisions;
	WeaponStash weaponStash;

	LocalDispatcher mEvents;
};