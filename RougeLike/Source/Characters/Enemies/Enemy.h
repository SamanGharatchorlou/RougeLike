#pragma once

#include "Events/Dispatcher.h"

#include "States/StateMachine.h"
#include "Animations/Animator.h"
#include "Collisions/Collider.h"

#include "EnemyPropertyBag.h"
#include "Characters/Attributes/Movement.h"
#include "EnemyEnums.h"

struct GameData;
class EnemyPropertyBag;


class Enemy : public Dispatcher
{
public:
	Enemy(GameData* gameData);
	~Enemy() { }

	void init(std::string name);
	void slowUpdate(float dt);
	void fastUpdate(float dt);
	void render();

	void addState(EnemyState state);
	void popState() { mStateMachine.popState(); }
	void replaceState(EnemyState state);

	virtual void resolvePlayerWeaponCollisions() = 0;

	const GameData* getData() { return mGameData; }

	void		setRect(RectF rect) { mRect = rect; }
	RectF&		getRect() { return mRect; }

	StateMachine* getStateMachine() { return &mStateMachine; }
	Animator*	getAnimator() { return &mAnimator; }
	Collider*	getCollider() { return &mCollider; }
	Movement&	getMovement() { return mMovement; }

	void		move(float dt);

	EnemyPropertyBag& propertyBag() { return bag; }

	void setTarget(RectF* rect) { mTarget = rect; }
	const RectF		targetRect() const { return *mTarget; }

	void spawn(VectorF position);

	virtual const EnemyType type() const = 0;

	Uint8& alpha() { return mAlpha; }

	void setActive(bool active) { mIsActive = active; }
	bool isActive() const { return mIsActive; }

	EnemyState getState() const { return mState.top(); }


private:
	void initAnimations(std::string name);


protected:
	GameData* mGameData;
	
	StateMachine mStateMachine;
	Collider mCollider;
	Animator mAnimator;

	Movement mMovement;
	RectF mRect; // world coords
	SDL_RendererFlip mFlip;

	EnemyPropertyBag bag;

	RectF* mTarget;

	Uint8 mAlpha;

	bool mIsActive;

	std::stack<EnemyState> mState;
};