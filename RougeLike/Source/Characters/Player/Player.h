#pragma once
#include "States/PlayerState.h" // TODO: only for enum actionstate type, could move it??

#include "Characters/Attributes/Physics.h"
#include "States/StateMachine.h"
#include "Animations/Animator.h"
#include "Collisions/Collider.h"
#include "Characters/Weapons/Weapon.h"

#include "PlayerPropertyBag.h"

struct GameData;
class Map;

class Player
{
public:
	Player(GameData* gameData);
	~Player() { }

	void init(std::string characterConfig);
	void processStateChanges();
	void handleInput();
	void slowUpdate(float dt);
	void fastUpdate(float dt);
	void render();

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

	Health health; // TODO: move this into the property bag

private:
	// Animations
	void selectAnimation(PlayerState::actionState state);
	void initAnimations(std::string config);

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

