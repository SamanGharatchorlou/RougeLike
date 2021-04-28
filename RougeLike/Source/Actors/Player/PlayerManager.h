#pragma once


#include "Player.h"
#include "Objects/Abilities/AbilityManager.h"
#include "Weapons/WeaponStash.h"
#include "Events/LocalDispatcher.h"
#include "Objects/Properties/Attributes/Levelling.h"
#include "Collisions/WallCollisionTracker.h"
#include "Collisions/WeaponCollisionTracker.h"
#include "Character.h"

class Environment;
class Screen;
enum class AbilityType;


class PlayerManager
{
public:
	PlayerManager();
	~PlayerManager();

	Player* get() { return &mPlayer; }

	void init(Environment* environment, Screen* gameScreen);
	void clear();

	void handleInput(const InputManager* input);
	void fastUpdate(float dt);
	void slowUpdate(float dt);
	void render();

	void handleEvents();
	LocalDispatcher& events() { return mEvents; }

	void resetColliders();

	void setPosition(VectorF position);
	void selectCharacter(const Character& character);
	void selectWeapon(const Character& character);

	void addAbility(AbilityType ability);
	void addExp(int exp);


	// test
	VectorF position() { return mPlayer.rectRef().LeftCenter(); }

private:
	Player mPlayer;
	Levelling mLevelling;
	AbilityManager mAbilities;

	WallCollisionTracker mWallCollisions;
	WeaponCollisionTracker mWeaponCollisions;

	WeaponStash mWeaponStash;

	LocalDispatcher mEvents;
};