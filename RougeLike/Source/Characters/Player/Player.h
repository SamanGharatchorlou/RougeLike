#pragma once
#include "States/PlayerState.h" // TODO: only for enum actionstate type, could move it??

#include "Characters/Attributes/Physics.h"
#include "States/StateMachine.h"
#include "Animations/Animator.h"
#include "Collisions/Collider.h"
#include "Characters/Weapons/Weapon.h"

#include "PlayerPropertyBag.h"

struct GameData;

class Player
{
public:
	Player(GameData* gameData);
	~Player() { }

	// TODO: change all 'std::string' parameters to 'const std::string&'
	void init(const std::string& characterConfig);
	void processStateChanges();
	void handleInput();
	void slowUpdate(float dt);
	void fastUpdate(float dt);
	void render();

	void equiptWeapon(const WeaponData* data) { mWeapon.equipt(data); }

	// Movement states
	void updateState();

	// Attack states
	void addState(PlayerState::actionState newState);
	void popState();

	RectF&		getRect()		{ return physics.getRect(); }
	Collider&	getCollider()	{ return mCollider; }
	Weapon&		getWeapon()		{ return mWeapon; }
	Physics&	getPhysics()	{ return physics; }

	PlayerPropertyBag& propertyBag() { return bag; }


private:
	// Animations
	void selectAnimation(PlayerState::actionState state);
	void initAnimations(const std::string& config);


private:
	GameData* mGameData;

	PlayerPropertyBag bag;

	StateMachine stateMachine;
	Animator mAnimator;
	Collider mCollider;
	Weapon mWeapon;
	Physics physics;

	SDL_RendererFlip mFlip;

	PlayerState::actionState mState;
};

