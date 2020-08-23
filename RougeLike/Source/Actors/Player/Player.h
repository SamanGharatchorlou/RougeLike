#pragma once

#include "Actors/Actor.h"

class Weapon;
class WeaponData;
class MeleeWeapon;
class AudioManager;


class Player : public Actor
{
	friend class PlayerCollisions;

public:
	Player();
	~Player() { };

	void setWeaponType(Weapon* weapon);
	void handleInput(const InputManager* input);
	void fastUpdate(float dt);
	void slowUpdate(float dt);
	void render();

	void clear();

	bool attemptAttack();
	void updateCursorPosition(VectorF cursorPosition);

	MeleeWeapon*	weapon();

	void setCharacter(const XMLNode playerNode);
	void selectWeapon(WeaponData* weapon);

	void overrideControl(bool removeControl);
	bool userHasControl() { return !mControlOverride; }

	void enableBodyCollisions(bool isEnabled);
	bool hasBodyCollisions() { return mBodyCollisions; }


	void updateWeaponHitSound(AudioManager* audio);
	void updateMapInfo(Map* map);


private:
	void processHit();


private:

	Vector2D<int> mTileIndex;
	int mMapLevel;

	Weapon* mWeapon;

	bool mControlOverride;
	bool mBodyCollisions;
};