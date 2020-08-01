#pragma once


#include "Player.h"
#include "Objects/Abilities/AbilityManager.h"
#include "Collisions/PlayerCollisions.h"
#include "Weapons/WeaponStash.h"
#include "Events/LocalDispatcher.h"

struct GameData;
class Environment;
class TextureManager;

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
	void selectCharacter(const std::string& characterConfig, TextureManager* textureManager);
	void selectWeapon(const std::string& weaponName);
	void addAbility(const std::string& ability);

private:
	Environment* mEnvironment;

	Player mPlayer;
	AbilityManager mAbilities;
	PlayerCollisions mPlayerCollisions;
	WeaponStash weaponStash;

	LocalDispatcher mEvents;
};