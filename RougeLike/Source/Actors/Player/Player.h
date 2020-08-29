#pragma once

#include "Actors/Actor.h"
#include "States/StateMachine.h"
#include "PlayerStates/PlayerState.h"
#include "States/State.h"


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

	void move(float dt);

	MeleeWeapon*	weapon();

	void setCharacter(const XMLNode playerNode);
	void selectWeapon(WeaponData* weapon);

	void overrideControl(bool removeControl);
	bool userHasControl() { return !mControlOverride; }

	void updateWeaponHitSound(AudioManager* audio);
	void updateMapInfo();

	void attack() override;
	bool isAttacking() const override;
	Collider* attackingCollider() override;

private:
	void processHit();
	void updateCursorPosition(VectorF cursorPosition);


private:
	StateMachine<PlayerState> mStateMachine;

	Vector2D<int> mTileIndex;
	int mMapLevel;

	Weapon* mWeapon;

	bool mControlOverride;
};