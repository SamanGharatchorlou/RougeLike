#pragma once

#include "Events/Dispatcher.h"

#include "States/StateMachine.h"
#include "Animations/Animator.h"
#include "Collisions/EnemyCollider.h"
#include "Characters/Attributes/Movement.h"

#include "EnemyPropertyBag.h"
#include "EnemyStates/EnemyState.h"
#include "EnemyEnums.h"


struct GameData;


class Enemy
{
public:
	Enemy(GameData* gameData);
	~Enemy() { }

	void init(std::string name);
	void slowUpdate(float dt);
	void fastUpdate(float dt);
	void render();
	void renderCharacter();

	void clear();

	// State handling
	void addState(EnemyState::Type state);
	void popState();
	void replaceState(EnemyState::Type state);
	EnemyState::Type state() const;

	void resolvePlayerWeaponCollisions();

	const GameData* getData() { return mGameData; }

	EnemyPropertyBag& propertyBag() { return bag; }

	// Event 
	const EventPacket popEvent();
	void pushEvent(const EventPacket event);
	bool hasEvent() const { return events.size() > 0; }

	void		setRect(RectF rect) { mRect = rect; }
	RectF&		getRect() { return mRect; }

	StateMachine<EnemyState>*	getStateMachine() { return &mStateMachine; }
	Animator*					getAnimator() { return &mAnimator; }
	DamageCollider*				getCollider() { return &mCollider; }
	Movement&					getMovement() { return mMovement; }

	void		move(float dt); // TODO: need this?

	void setTarget(RectF* rect) { mTarget = rect; }
	const RectF		targetRect() const { return *mTarget; }

	void setFlip(SDL_RendererFlip flip) { mFlip = flip; }
	SDL_RendererFlip flip() const { return mFlip; }

	void spawn(VectorF position);

	virtual const EnemyType type() const = 0;


private:
	void initAnimations(std::string name);


protected:
	GameData* mGameData;
	
	StateMachine<EnemyState> mStateMachine;

	DamageCollider mCollider;
	Animator mAnimator;

	Movement mMovement;
	RectF mRect; // world coords
	SDL_RendererFlip mFlip;

	EnemyPropertyBag bag;

	RectF* mTarget;

	std::queue<EventPacket> events;
};