#pragma once
#include "States/PlayerState.h" // TODO: only for enum actionstate type, could move it??

#include "Characters/Attributes/Movement.h"

#include "States/StateMachine.h"
#include "Animations/Animator.h"
#include "Collisions/Collider.h"
#include "Characters/Weapons/Weapon.h"

struct GameData;
class Map;

class Player //: public Actor
{
public:
	Player(GameData* gameData);
	~Player() { }

	void init();
	void processStateChanges();
	void handleInput();
	void slowUpdate(float dt);
	void fastUpdate(float dt);
	void render();

	void resolveWallCollisions(float dt);

	// Movement states
	void updateState();

	// Attack states
	void addState(PlayerState::actionState newState);
	void popState();

	Movement&		getMovement()	{ return mMovement; }
	const Movement& getMovement() const { return mMovement; }

	RectF&		getRect()		{ return mRect; }
	Collider&	getCollider()	{ return mCollider; }
	Weapon&		getWeapon()		{ return mWeapon; }


private:
	// Animations
	void selectAnimation(PlayerState::actionState state);
	void initAnimations(std::string config);

	// Wall collisions
	bool doesCollideLeft(VectorF point, float dt);
	bool doesCollideRight(VectorF point, float dt);
	bool doesCollideTop(VectorF point, float dt);
	bool doesCollideBot(VectorF point, float dt);


private:
	GameData* mGameData;

	StateMachine stateMachine;
	Animator mAnimator;
	Collider mCollider;
	Weapon mWeapon;

	Movement mMovement;
	RectF mRect;
	SDL_RendererFlip mFlip;

	PlayerState::actionState mState;
};

