#include "pch.h"
#include "Enemy.h"

#include "Graphics/Texture.h"
#include "EnemyStates/EnemyStateHeaders.h"

#include "Game/Environment.h"
#include "AI/Pathing/AIPathMap.h"

#include "Objects/Pools/EnemyStatePool.h"
#include "Game/Camera/Camera.h"

// TEMP
#include "Map/Map.h"
#include "Objects/Abilities/AbilityClasses/Ability.h"


Enemy::Enemy() :
	mStateMachine(new EnemyNullState),
	mTarget(nullptr),
	mStatePool(nullptr)
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


void Enemy::initAbilities(std::vector<Actor*>* targets)
{
	mAbilities.init(mEffects.pool(), targets);
}


void Enemy::fastUpdate(float dt)
{
	mPhysics.resetHasForce();
	mPhysics.stopAcceleration();

	mStateMachine.getActiveState().fastUpdate(dt);

	mAbilities.fastUpdate(dt);

	Actor::fastUpdate(dt);

	mPhysics.move(dt);
}


void Enemy::slowUpdate(float dt)
{
	// Must run first, some effects e.g. damage apply a gotHit state to the collider
	Actor::slowUpdate(dt);

	EnemyState* state = mStateMachine.processStateChanges();
	if (state)
	{
		mStatePool->returnObject(state);
	}
	
	resolveCollisions();
	mStateMachine.getActiveState().slowUpdate(dt);

	mAbilities.slowUpdate(dt);

	if (mAIPathing.updateCurrentIndex(position()))
	{
		UpdateAICostMapEvent* event = new UpdateAICostMapEvent;
		mEvents.push(EventPacket(event));
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
	const float flashTime = 0.1f;
	if (mColourModTimer.isStarted() && mColourModTimer.getSeconds() < flashTime)
	{
		RenderColour colourMod = RenderColour(225, 0, 0);
		mAnimator.render(renderRect(), mPhysics.flip(), colourMod);
	}
	else
	{
		mColourModTimer.stop();
		mAnimator.render(renderRect(), mPhysics.flip());
	}
}


RectF Enemy::renderRect() const
{
	VectorF offset = mRenderOffset;
	if (mPhysics.flip() == SDL_FLIP_HORIZONTAL)
	{
		offset = VectorF(-mRenderOffset.x, mRenderOffset.y);
	}

	RectF renderRect = Camera::Get()->toCameraCoords(rect());
	return renderRect.Translate(offset);
}


void Enemy::clear()
{
	mEvents.clear();
	mAIPathing.clear();
	Actor::clear();
}


void Enemy::resolveCollisions()
{
	if (mCollider.gotHit())
	{
		mColourModTimer.restart();
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


void Enemy::addState(EnemyState::Type newState)
{
	if (state() != EnemyState::Dead && state() != EnemyState::Stun && state() != EnemyState::Exit)
	{
		handleQueuedStates();
		mStateMachine.addState(getNewState(newState));
	}
}


void Enemy::replaceState(EnemyState::Type newState)
{
	handleQueuedStates();
	mStateMachine.replaceState(getNewState(newState));
}


void Enemy::popState()
{
	mStateMachine.popState();
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


// -- Attacking
void Enemy::attack()
{
	replaceState(EnemyState::Attack);
}


bool Enemy::isAttacking() const
{
	EnemyState::Type currentState = state();
	return currentState == EnemyState::Attack || currentState == EnemyState::PreAttack;
}


Collider* Enemy::attackingCollider()
{
	return &mCollider;
}


void Enemy::resetColliders()
{
	mCollider.reset();
}


// -- Private Functions --
EnemyState* Enemy::getNewState(EnemyState::Type type)
{
	EnemyState* state = mStatePool->getObject(type);
	state->set(this);
	state->enter();

	return state;
}

void Enemy::handleQueuedStates()
{
	if (mStateMachine.hasQueuedState())
	{
		EnemyState* queuedEnemyState = mStateMachine.queuedState();
		if(queuedEnemyState)
			mStatePool->returnObject(queuedEnemyState);
	}
}