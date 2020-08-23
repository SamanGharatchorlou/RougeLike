#pragma once


#include "Player.h"
#include "Objects/Abilities/AbilityManager.h"
#include "Collisions/PlayerCollisions.h"
#include "Weapons/WeaponStash.h"
#include "Events/LocalDispatcher.h"
#include "Objects/Properties/Attributes/Levelling.h"

struct GameData;
class Environment;
class CollisionManager;
class Screen;
enum class AbilityType;


class PlayerManager
{
public:
	PlayerManager();
	~PlayerManager();

	Player* get() { return &mPlayer; }

	void init(Environment* environment, CollisionManager* collisions, Screen* gameScreen);
	void clear();

	void handleInput(const InputManager* input);
	void fastUpdate(float dt);
	void slowUpdate(float dt);
	void render();

	void handleEvents();
	LocalDispatcher& events() { return mEvents; }

	void setPosition(VectorF position);
	void selectCharacter(const BasicString& characterConfig);
	void selectWeapon(const BasicString& weaponName);

	void addAbility(AbilityType ability);

	void addExp(int exp);


private:
	Environment* mEnvironment;

	Player mPlayer;
	Levelling mLevelling;
	AbilityManager mAbilities;
	PlayerCollisions mPlayerCollisions;
	WeaponStash mWeaponStash;

	LocalDispatcher mEvents;
};