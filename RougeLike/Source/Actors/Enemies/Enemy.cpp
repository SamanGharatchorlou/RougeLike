#include "pch.h"
#include "Enemy.h"

#include "Graphics/Texture.h"
#include "Collisions/EffectCollider.h"
#include "EnemyStates/EnemyStateHeaders.h"

#include "Game/Environment.h"
#include "AI/AIPathMap.h"

#include "Objects/Pools/EnemyStatePool.h"

// TEMP
#include "Map/Map.h"


Enemy::Enemy() :
	mStateMachine(new EnemyNullState),
	mTarget(nullptr),
	mStatePool(nullptr),
	mCurrentIndex(Vector2D<int>(-1,-1))
#if _DEBUG
	, mDebugger(this)
#endif
{
	physics()->setFlip(static_cast<SDL_RendererFlip>(randomNumberBetween(0, 2)));
}

// REQUIRES TESTING
Enemy::~Enemy()
{
	if (mStateMachine.size() != 1 || mStateMachine.getActiveState().type() != EnemyState::None)
	{
		DebugPrint(Warning, "There should only be a single EnemyNullState left at this point\n");
	}
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

	EnemyState* state = mStateMachine.processStateChanges();
	if (state)
		mStatePool->returnObject(state, state->type());

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
#if _DEBUG
	mDebugger.draw();
#endif

	VectorF offset = mRenderOffset;
	if (mPhysics.flip() == SDL_FLIP_NONE)
	{
		offset = VectorF(-mRenderOffset.x, mRenderOffset.y);;
	}

	Actor::render(offset);
}

// TODO: remove this access?
const Map* Enemy::getEnvironmentMap() const
{
	return mEnvironment->map(MapType::Dungeon);
}


// Reset everything that needs to be recalculated when spawned
void Enemy::clear()
{
	// REQUIRES TESTING
	while (mStateMachine.size() > 1)
	{
		popState();
		EnemyState* state = mStateMachine.processStateChanges();
		mStatePool->returnObject(state, state->type());
	}

	mEvents.clear();
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


void Enemy::replaceState(EnemyState::Type newState)
{
	EnemyState* state = mStatePool->getObject(newState);
	state->set(this);

	mStateMachine.replaceState(state);
}


void Enemy::stun(float stunTime)
{
	if (state() != EnemyState::Dead)
	{
		EnemyState* state = mStatePool->getObject(EnemyState::Stun);
		EnemyStun* stunState = static_cast<EnemyStun*>(state);
		stunState->setTime(stunTime);

		stunState->set(this);
		mStateMachine.addState(stunState);
	}
}


void Enemy::idle(float idleTime)
{
	if (state() != EnemyState::Dead)
	{
		EnemyState* state = mStatePool->getObject(EnemyState::Idle);
		EnemyIdle* idleState = static_cast<EnemyIdle*>(state);
		idleState->setTime(idleTime);

		idleState->set(this);
		mStateMachine.addState(idleState);
	}
}


void Enemy::addState(EnemyState::Type newState)
{
	if (state() != EnemyState::Dead)
	{
		EnemyState* state = mStatePool->getObject(newState);
		state->set(this);
		state->enter();
		mStateMachine.addState(state);
	}
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
