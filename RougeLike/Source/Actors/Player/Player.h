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

	void selectWeapon(WeaponData* weapon);

	void overrideControl(bool removeControl);
	bool userHasControl() { return !mControlOverride; }

	void updateMapInfo();

	void attack() override;
	bool isAttacking() const override;
	Collider* attackingCollider() override;
	void resetColliders() override;


private:
	void updateMovementSound(AudioManager* audio);
	void setMovementAnimation();
	void handleStates(float dt);
	void handleHealthChanges(Health* health);
	void handleHit(AudioManager* audio);

	void updateCursorPosition(VectorF cursorPosition);


private:
	StateMachine<PlayerState> mStateMachine;

	int mMapLevel;

	Weapon* mWeapon;

	bool mControlOverride;
};