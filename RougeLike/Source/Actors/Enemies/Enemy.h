#pragma once

#include "Actors/Actor.h"
#include "States/StateMachine.h"
#include "EnemyStates/EnemyState.h"
#include "Actors/Enemies/Types/EnemyTypes.h"

#include "AI/Pathing/AIPathing.h"

#include "Objects/Abilities/AbilityHandler.h"

#if DEBUG_CHECK
#include "EnemyDebugger.h"
#endif


class EnemyStatePool;
enum class EnemyType;


class Enemy : public Actor
{
public:
	Enemy();
	~Enemy();

	void initAbilities(std::vector<Actor*>* target);
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
	StateMachine<EnemyState>* getStateMachine() { return &mStateMachine; }
	void addState(EnemyState::Type state);
	void popState();
	void replaceState(EnemyState::Type state);
	EnemyState::Type state() const;

	void idle(float idleTime);
	void stun(float stunTime);

	// Abilities
	AbilityHandler& abilities() { return mAbilities; }

	// Collisions
	void resolveCollisions();

	// Target
	void			setTarget(Actor* target) { mTarget = target; }
	const Actor*	target() const { return mTarget; }
	bool			hasTarget() const { return mTarget != nullptr; }

	// Pathing
	const AIPathing* pathing() const { return &mAIPathing; }
	void			accellerateTowards(VectorF position);
	void			move(VectorF velocity, float dt) { mPhysics.move(velocity, dt); }

	// Attacking
	virtual void attack() override;
	virtual bool canAttck() const { return true; }

	bool isAttacking() const override;
	Collider* attackingCollider() override;
	void resetColliders() override;

#if DEBUG_CHECK
	EnemyDebugger mDebugger;
#endif


private:
	void handleQueuedStates();
	EnemyState* getNewState(EnemyState::Type type);



protected:
	AIPathing mAIPathing;

	EnemyStatePool* mStatePool;
	StateMachine<EnemyState> mStateMachine;

	AbilityHandler mAbilities;

	const Actor* mTarget;
};