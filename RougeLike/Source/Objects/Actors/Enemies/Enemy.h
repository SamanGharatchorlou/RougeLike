#pragma once

#include "States/StateMachine.h"
#include "EnemyStates/EnemyState.h"
#include "EnemyEnums.h"

#include "../Actor.h"

#include "Events/LocalDispatcher.h"


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

	void addWaitState(float waitTime);
	 
	// Map
	void setMap(AIPathMap* map) { mMap = map; }
	AIPathMap*					getPathMap() { return mMap; }
	const Map*					getEnvironmentMap() const;

	// Collisions
	void resolveCollisions();

	// Target
	void			setTarget(Actor* target) { mTarget = target; }
	const Actor*	target() const { return mTarget; }

	// Movement
	void			accellerateTowards(VectorF position);
	void			move(VectorF velocity, float dt) { mPhysics.move(velocity, dt); }


protected:
	void init(const std::string& config);


protected:
	AIPathMap* mMap;

	StateMachine<EnemyState> mStateMachine;

	const Actor* mTarget;

	Index mCurrentIndex;
};