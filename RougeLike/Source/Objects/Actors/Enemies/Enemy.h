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
	 
	// Map
	void setMap(AIPathMap* map) { mMap = map; }
	AIPathMap*					getPathMap() { return mMap; }
	const Map*					getEnvironmentMap() const;

	// Collisions
	void resolveCollisions();

	// Target
	void			setAttackTarget(const RectF* rect) { mAttackTarget = rect; }
	const RectF*	attackTargetRect() const { return mAttackTarget; }
	void			setPositionTarget(const RectF* rect) { mPositionTarget = rect; }
	const RectF*	positionTargetRect() const { return mPositionTarget; }

	// Movement
	void			accellerateTowards(VectorF position);
	void			move(VectorF velocity, float dt) { mPhysics.move(velocity, dt); }


protected:
	void init(const std::string& config);
	void initAnimations(const std::string& config);


protected:
	AIPathMap* mMap;

	StateMachine<EnemyState> mStateMachine;

	const RectF* mAttackTarget;
	const RectF* mPositionTarget;

	VectorF mColliderRatio;

	Index mCurrentIndex;
};