#pragma once

#include "Actors/Actor.h"
#include "States/StateMachine.h"
#include "EnemyStates/EnemyState.h"
#include "Types/EnemyTypes.h"

#include "AI/AIPathing.h"

#if _DEBUG
#include "EnemyDebugger.h"
#endif


struct GameData;
class AIPathMap;
class Map;
class DamageCollider;
class Environment;


class Enemy : public Actor
{
public:
	Enemy();
	~Enemy() { }

	virtual void init() = 0;

	// Core
	void slowUpdate(float dt);
	void fastUpdate(float dt);
	void render();

	void renderCharacter();

	void clear();

	virtual const EnemyType type() const = 0;

	void spawn(EnemyState::Type state, VectorF position, const AIPathMap* map);

	// State handling
	const StateMachine<EnemyState>*	getStateMachine() const { return &mStateMachine; }
	void addState(EnemyState::Type state);
	void popState();
	void replaceState(EnemyState::Type state);
	EnemyState::Type state() const;

	void addIdleState(float waitTime);
	void stun(float stunTime);

	// Map
	const AIPathing& getPathMap() const { return mAIPathing; }
	const Map* getEnvironmentMap() const;

	// Collisions
	void resolveCollisions();

	// Target
	void			setTarget(Actor* target) { mTarget = target; }
	const Actor*	target() const { return mTarget; }
	bool			hasTarget() const { return mTarget != nullptr; }

	// Movement
	void			accellerateTowards(VectorF position);
	void			move(VectorF velocity, float dt) { mPhysics.move(velocity, dt); }


protected:
	AIPathing mAIPathing;

	StateMachine<EnemyState> mStateMachine;

	const Actor* mTarget;

	Index mCurrentIndex;

	std::deque<Effect*> mAttackEffects;

#if _DEBUG
	EnemyDebugger mDebugger;
#endif
};