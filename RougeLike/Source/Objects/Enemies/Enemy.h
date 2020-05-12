#pragma once

#include "States/StateMachine.h"
#include "EnemyStates/EnemyState.h"
#include "EnemyEnums.h"

#include "Objects/Actor.h"

#include "Events/Dispatcher.h"


struct GameData;
class AIPathMap;
class Map;
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

	void clear();

	virtual const EnemyType type() const = 0;

	void spawn(EnemyState::Type state, VectorF position);

	// State handling
	StateMachine<EnemyState>*	getStateMachine() { return &mStateMachine; }
	void addState(EnemyState::Type state);
	void popState();
	void replaceState(EnemyState::Type state);
	EnemyState::Type state() const;

	// Events
	const EventPacket popEvent();
	void pushEvent(const EventPacket event) { mEvents.push(event); }
	bool hasEvent() const { return mEvents.size() > 0; }
	 
	// Map
	void setMap(AIPathMap* map) { mMap = map; }
	AIPathMap*					getPathMap() { return mMap; }
	const Map*					getEnvironmentMap() const;

	// Collisions
	void resolvePlayerWeaponCollisions();

	// Target
	void			setAttackTarget(const RectF* rect) { mAttackTarget = rect; }
	const RectF*	attackTargetRect() const { return mAttackTarget; }
	void			setPositionTarget(const RectF* rect) { mPositionTarget = rect; }
	const RectF*	positionTargetRect() const { return mPositionTarget; }

	// Movement
	void			accellerateTowards(VectorF position);
	void			move(VectorF velocity, float dt) { mPhysics.move(velocity, dt); }


protected:
	void initAnimations(const std::string& name);


protected:
	AIPathMap* mMap;
	
	StateMachine<EnemyState> mStateMachine;

	const RectF* mAttackTarget;
	const RectF* mPositionTarget;

	std::queue<EventPacket> mEvents;

	VectorF mColliderRatio;
};