#include "pch.h"
#include "Enemy.h"

#include "Graphics/Texture.h"
#include "Collisions/Colliders/EffectCollider.h"
#include "EnemyStates/EnemyStateHeaders.h"

#include "Game/Environment.h"
#include "AI/AIPathMap.h"

#include "Objects/Pools/EnemyStatePool.h"
#include "Game/Camera/Camera.h"

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
	mPhysics.stopAcceleration();

	mStateMachine.getActiveState().fastUpdate(dt);

	Actor::fastUpdate(dt);

	mPhysics.move(dt);
}


void Enemy::slowUpdate(float dt)
{
	// Must run first, some effects e.g. damage apply a gotHit state to the collider
	Actor::slowUpdate(dt);

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
	if (mVisibility)
	{
#if _DEBUG
		mDebugger.draw();
#endif

		mStateMachine.getActiveState().render();
		mEffects.render();
	}
}


void Enemy::renderCharacter()
{
	mAnimator.render(renderRect(), mPhysics.flip());
}


RectF Enemy::renderRect() const
{
	VectorF offset = mRenderOffset;
	if (mPhysics.flip() == SDL_FLIP_HORIZONTAL)
	{
		offset = VectorF(-mRenderOffset.x, mRenderOffset.y);;
	}

	RectF renderRect = Camera::Get()->toCameraCoords(rect());
	return renderRect.Translate(offset);
}


void Enemy::clear()
{
	while (mStateMachine.size() > 1)
	{
		EnemyState* state = mStateMachine.forcePop();
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


void Enemy::spawn(EnemyState::Type state, VectorF position, AIPathMap* map)
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
