#pragma once

#include "Actors/Actor.h"
#include "States/StateMachine.h"
#include "EnemyStates/EnemyState.h"
#include "Actors/Enemies/Types/EnemyTypes.h"


#include "AI/Pathing/AIPathing.h"


#if _DEBUG
#include "EnemyDebugger.h"
#endif


class EnemyStatePool;
enum class EnemyType;


class Enemy : public Actor
{
public:
	Enemy();
	~Enemy();

	virtual void init() = 0;

	void setStatePool(EnemyStatePool* pool) { mStatePool = pool; }

	// Core
	void slowUpdate(float dt);
	void fastUpdate(float dt);

	void render();
	void renderCharacter();
	RectF renderRect() const;
	VectorF renderOffset() const { return mRenderOffset; }

	void clear();

	virtual const EnemyType type() const = 0;

	void spawn(EnemyState::Type state, VectorF position, const AIPathMap* map);

	// State handling
	const StateMachine<EnemyState>*	getStateMachine() const { return &mStateMachine; }
	void addState(EnemyState::Type state);
	void popState();
	void replaceState(EnemyState::Type state);
	EnemyState::Type state() const;

	void idle(float idleTime);
	void stun(float stunTime);

	// Map
	const AIPathing* pathing() const { return &mAIPathing; }
	//AIPathing* getAIPathing() { return &mAIPathing; }

	// Collisions
	void resolveCollisions(bool addHitState = true);

	// Target
	void			setTarget(Actor* target) { mTarget = target; }
	const Actor*	target() const { return mTarget; }
	bool			hasTarget() const { return mTarget != nullptr; }

	// Movement
	void			accellerateTowards(VectorF position);
	void			move(VectorF velocity, float dt) { mPhysics.move(velocity, dt); }

#if _DEBUG
	EnemyDebugger mDebugger;
#endif

protected:
	AIPathing mAIPathing;

	EnemyStatePool* mStatePool;
	StateMachine<EnemyState> mStateMachine;

	const Actor* mTarget;

	TimerF mColourModTimer;
};