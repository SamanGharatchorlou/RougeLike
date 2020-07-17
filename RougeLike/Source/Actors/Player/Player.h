#pragma once

#include "Actors/Actor.h"
//#include "Objects/Abilities/AbilityManager.h"


struct GameData;
class MeleeWeapon;
class AudioManager;

class Player : public Actor
{
	friend class PlayerCollisions;

public:
	Player(GameData* gameData);
	~Player() { };

	void init(const std::string& characterConfig);
	void handleInput(const InputManager* input);
	void fastUpdate(float dt);
	void slowUpdate(float dt);
	void render();

	void reset();

	bool attemptAttack();
	void updateCursorPosition(VectorF cursorPosition);

	MeleeWeapon*	weapon();

	void selectCharacter(const std::string& character);
	void setWeapon(MeleeWeapon* weapon);

	void addAbility(const std::string& name);

	void overrideControl(bool removeControl);
	bool userHasControl() { return !mControlOverride; }

	void updateUI();

	void enableBodyCollisions(bool isEnabled);
	bool hasBodyCollisions() { return mBodyCollisions; }


	void updateWeaponHitSound(AudioManager* audio);
	void updateCurrentTile(Map* map);
private:
	void processHit();




private:
	//AbilityManager mAbilities;

	Vector2D<int> tileIndex;

	MeleeWeapon* mWeapon;

	bool mControlOverride;
	bool mBodyCollisions;
};