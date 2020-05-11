#pragma once

#include "States/StateMachine.h"

#include "Objects/Attributes/Physics.h"

#include "EnemyPropertyBag.h"
#include "EnemyStates/EnemyState.h"
#include "EnemyEnums.h"

#include "Objects/Actor.h"

#include "Events/Dispatcher.h"


struct GameData;
class AIPathMap;
class DamageCollider;


class Enemy : public Actor
{
public:
	Enemy(GameData* gameData);
	~Enemy() { }

	// Core
	virtual void init(const std::string& characterConfig);
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
	AIPathMap*					getMap() { return mMap; }

	DamageCollider*				damageCollider() const;

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
	AIPathMap* mMap;
	
	StateMachine<EnemyState> mStateMachine;

	const RectF* mAttackTarget;
	const RectF* mPositionTarget;

	std::queue<EventPacket> events;

	VectorF colliderRatio;
};