#pragma once

#include "Actors/Actor.h"
#include "States/StateMachine.h"
#include "EnemyStates/EnemyState.h"
#include "Types/EnemyTypes.h"



struct GameData;
class AIPathMap;
class Map;
class DamageCollider;
class Environment;
class TextureManager;


class Enemy : public Actor
{
public:
	Enemy();
	~Enemy() { }

	virtual void init(TextureManager* textureManager, EffectPool* effects) = 0;

	// Core
	void slowUpdate(float dt);
	void fastUpdate(float dt);
	void render();

	void renderCharacter();

	void clear();

	virtual const EnemyType type() const = 0;

	void spawn(EnemyState::Type state, VectorF position);

	// State handling
	const StateMachine<EnemyState>*	getStateMachine() const { return &mStateMachine; }
	void addState(EnemyState::Type state);
	void popState();
	void replaceState(EnemyState::Type state);
	EnemyState::Type state() const;

	void addWaitState(float waitTime);
	void addIdleState(float waitTime);
	 
	// Map
	void setMap(const AIPathMap* map) { mMap = map; }
	const AIPathMap* getPathMap() { return mMap; }
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

	// Effects
	//void addAttackingEffect(Effect* effect) { mAttackEffects.push_front(effect); }


	void readEffects(const XMLParser& parser, EffectPool* effects);

protected:
	// TODO: change all parser parameters to const


protected:
	const AIPathMap* mMap;

	StateMachine<EnemyState> mStateMachine;

	const Actor* mTarget;

	Index mCurrentIndex;

	std::deque<Effect*> mAttackEffects;
};