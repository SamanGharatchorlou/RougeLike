#pragma once

#include "Events/Dispatcher.h"

#include "States/StateMachine.h"
#include "Animations/Animator.h"
#include "Collisions/EnemyCollider.h"

//#include "Objects/Attributes/Movement.h"
#include "Objects/Attributes/Physics.h"

#include "EnemyPropertyBag.h"
#include "EnemyStates/EnemyState.h"
#include "EnemyEnums.h"


struct GameData;
class AIPathMap;

class Enemy
{
public:
	Enemy(GameData* gameData);
	~Enemy() { }

	// Core
	virtual void init();
	void slowUpdate(float dt);
	void fastUpdate(float dt);
	void render();
	void renderCharacter();

	void setMap(AIPathMap* map) { mMap = map; }

	// State handling
	void addState(EnemyState::Type state);
	void popState();
	void replaceState(EnemyState::Type state);
	EnemyState::Type state() const;

	void clear();

	// Data
	const GameData* getData() { return mGameData; }
	EnemyPropertyBag& propertyBag() { return mBag; }
	virtual const EnemyType type() const = 0;

	// Events
	const EventPacket popEvent();
	void pushEvent(const EventPacket event);
	bool hasEvent() const { return events.size() > 0; }

	// Rect
	RectF&		rect() { return mPhysics.rectRef(); }
	VectorF		position() const { return mPhysics.position(); }

	virtual RectF renderRect() const = 0;
	 
	// Systems
	StateMachine<EnemyState>*	getStateMachine() { return &mStateMachine; }
	Animator*					getAnimator() { return &mAnimator; }
	DamageCollider*				collider() { return &mCollider; }
	Physics&					physics() { return mPhysics; }
	AIPathMap*					getMap() { return mMap; }

	// Dynamics
	void resolvePlayerWeaponCollisions();

	// Target
	void			setAttackTarget(const RectF* rect) { mAttackTarget = rect; }
	const RectF*	attackTargetRect() const { return mAttackTarget; }

	void			setPositionTarget(const RectF* rect) { mPositionTarget = rect; }
	const RectF*	positionTargetRect() const { return mPositionTarget; }

	VectorF			directionTo(VectorF position) const { return position - mPhysics.position(); }
	void			accellerateTowards(VectorF position);
	void			move(VectorF velocity, float dt) { mPhysics.move(velocity, dt); }

	void facePoint(VectorF point) { mPhysics.facePoint(point); }
	void setFlip(SDL_RendererFlip flip) { mPhysics.setFlip(flip); }
	SDL_RendererFlip flip() const { return mPhysics.flip(); }

	void spawn(EnemyState::Type state, VectorF position);


protected:
	void initAnimations(const std::string& name);


protected:
	GameData* mGameData;
	AIPathMap* mMap;
	
	StateMachine<EnemyState> mStateMachine;
	DamageCollider mCollider;
	Animator mAnimator;
	Physics mPhysics;

	EnemyPropertyBag mBag;

	const RectF* mAttackTarget;
	const RectF* mPositionTarget;

	std::queue<EventPacket> events;

	VectorF colliderRatio;
};