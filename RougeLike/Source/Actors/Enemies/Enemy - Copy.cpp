#include "pch.h"
#include "Enemy.h"

#include "Graphics/Texture.h"
#include "Collisions/EffectCollider.h"
#include "EnemyStates/EnemyStateHeaders.h"

#include "Game/Environment.h"
#include "AI/AIPathMap.h"

// TEMP
#include "Map/Map.h"

#if _DEBUG
#include "Debug/DebugDraw.h"
#endif


Enemy::Enemy() :
	mStateMachine(new EnemyNullState)
#if _DEBUG
	, mDebugger(this)
#endif
{
	physics()->setFlip(static_cast<SDL_RendererFlip>(randomNumberBetween(0, 2)));
}


void Enemy::fastUpdate(float dt)
{
	mPhysics.resetHasForce();

	mStateMachine.getActiveState().fastUpdate(dt);

	Actor::fastUpdate(dt);
}


void Enemy::slowUpdate(float dt)
{
	// Must run first, some effects e.g. damage apply a gotHit state to the collider
	Actor::slowUpdate(dt);

	// Reset alpha for enemies sharing the same texture
	mAnimator.texture()->setAlpha(alphaMax);

	mStateMachine.processStateChanges();
	mStateMachine.getActiveState().slowUpdate(dt);

	Index index = mAIPathing.index(position());
	if (mCurrentIndex != index)
	{
		UpdateAICostMapEvent* event = new UpdateAICostMapEvent;
		mEvents.push(EventPacket(event));
		mCurrentIndex = index;
	}

	Health* health = static_cast<Health*>(getAttribute(AttributeType::Health));
	if (health->isDead())
		addState(EnemyState::Dead);
}


void Enemy::render()
{
	if(mVisibility)
		mStateMachine.getActiveState().render();
}


void Enemy::renderCharacter()
{
	Actor::render();

#if _DEBUG
	mDebugger.draw();
#endif
}

// TODO: remove this access?
const Map* Enemy::getEnvironmentMap() const
{
	return mEnvironment->map(MapType::Dungeon);
}


// Reset everything that needs to be recalculated when spawned
void Enemy::clear()
{
	// Clear statemachine states (except null state at i = 0)
	mStateMachine.shallowClear();
	mEvents.clear();

	//mMap = nullptr;
	mAIPathing.clear();

	Actor::clear();
}


void Enemy::resolveCollisions()
{
	if (mCollider.gotHit())
	{
		// Player weapon hit enemy
		if (mCollider.getOtherCollider())
		{
			EffectCollider* effectCollider = static_cast<EffectCollider*>(mCollider.getOtherCollider());
			handleEffects(effectCollider);
		}

		addState(EnemyState::Hit);
	}
}


void Enemy::spawn(EnemyState::Type state, VectorF position, const AIPathMap* map)
{
	mAIPathing.init(map);

	mPhysics.setPosition(position);

	addState(state);
	mStateMachine.processStateChanges();
}


void Enemy::accellerateTowards(VectorF position)
{
	mPhysics.accellerate(position - rect().Center());
}


void Enemy::replaceState(EnemyState::Type state)
{
	mStateMachine.replaceState(newEnemyState(state, this));
}


void Enemy::addIdleState(float waitTime)
{
	if (state() != EnemyState::Dead)
		mStateMachine.addState(new EnemyIdle(this, waitTime));
}


void Enemy::stun(float stunTime)
{
	if (state() != EnemyState::Dead)
		mStateMachine.addState(new EnemyStun(this, stunTime));
}


void Enemy::addState(EnemyState::Type newState)
{
	if (state() != EnemyState::Dead)
		mStateMachine.addState(newEnemyState(newState, this));
}


void Enemy::popState()
{
	mStateMachine.popState();
}


EnemyState::Type Enemy::state() const
{
	if (mStateMachine.size() > 0)
	{
		return mStateMachine.getActiveState().type();
	}
	else
	{
		DebugPrint(Warning, "Enemy state machine has no state, size = 0\n");
		return EnemyState::None;
	}
}