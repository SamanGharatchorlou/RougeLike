#include "pch.h"
#include "Enemy.h"

#include "Game/GameData.h"
#include "EnemyStates/EnemyStateHeaders.h"

#include "Graphics/Texture.h"

#include "Map/Environment.h"

#include "Collisions/EffectCollider.h"

// temp
#include "Objects/Effects/EffectPool.h"


#include "AI/AIPathMap.h"

#if _DEBUG
#include "Debug/DebugDraw.h"
#endif


Enemy::Enemy(GameData* gameData) :
	Actor(gameData),
	mStateMachine(new EnemyNullState),
	mMap(nullptr),
	mCurrentIndex(Vector2D<int>(-1,-1))
{
	physics()->setFlip(static_cast<SDL_RendererFlip>(randomNumberBetween(0, 2)));
}


void Enemy::init(const std::string& config)
{
	Actor::init(config);

	mEffects.addAttackingEffect(EffectType::Damage);
	mEffects.addAttackingEffect(EffectType::Displacement);

	setEffectProperty("Damage", getPropertyValue("Damage"));
	setEffectProperty("KnockbackForce", getPropertyValue("KnockbackForce"));
	setEffectProperty("KnockbackDistance", getPropertyValue("KnockbackDistance"));
}


void Enemy::fastUpdate(float dt)
{
	mPhysics.resetHasForce();

	mStateMachine.getActiveState().fastUpdate(dt);

	Actor::fastUpdate(dt);
}


void Enemy::effectLoop()
{
	if (mCollider.hasEffects())
	{
		printf("enemy has %d effects\n", mCollider.effectCount());
	}

	if (mCollider.didHit())
	{
		// HACK: V V V V V - Update knockback source
		mEffectProperties.setProperty("TargetPositionX", position().x);
		mEffectProperties.setProperty("TargetPositionY", position().y);
		// HACK: ^ ^ ^ ^ ^

		const std::vector<EffectType> effects = mEffects.attackingEffects();

		// Add effects to collider
		for (int i = 0; i < effects.size(); i++)
		{
			Effect* effect = mGameData->effectPool->getEffect(effects[i]);
			effect->fillData(&mEffectProperties);
			mCollider.addEffect(effect);
		}
	}
}


void Enemy::slowUpdate(float dt)
{
	// Must run first, some effects e.g. damage apply a gotHit state to the collider
	Actor::slowUpdate(dt);

	// Reset alpha for enemies sharing the same texture
	mAnimator.texture()->setAlpha(alphaMax);

	mStateMachine.processStateChanges();
	mStateMachine.getActiveState().slowUpdate(dt);

	Index index = mMap->index(position());
	if (mCurrentIndex != index)
	{
		UpdateAICostMapEvent* event = new UpdateAICostMapEvent;
		pushEvent(EventPacket(event));
		mCurrentIndex = index;
	}

	Health* health = static_cast<Health*>(getProperty("Health"));
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
#if DRAW_ENEMY_RECTS
	debugDrawRect(rect(), RenderColour(RenderColour::Red));
	debugDrawRect(scaledRect(), RenderColour(RenderColour::Blue));
#endif	
	
	Actor::render();
}

const Map* Enemy::getEnvironmentMap() const
{
	return mGameData->environment->primaryMap();
}


// Reset everything that needs to be recalculated when spawned
void Enemy::clear()
{
	// Clear statemachine states (except null state at i = 0)
	mStateMachine.clearStates();
	mEvents.clear();

	mMap = nullptr;

	Actor::reset();
}


void Enemy::resolveCollisions()
{
	if (mCollider.gotHit())
	{
		// Player weapon hit enemy
		if (mCollider.getOtherCollider())
		{
			EffectCollider* effectCollider = static_cast<EffectCollider*>(mCollider.getOtherCollider());
			processEffects(effectCollider);
		}

		addState(EnemyState::Hit);
	}
}


void Enemy::spawn(EnemyState::Type state, VectorF position)
{
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


void Enemy::addWaitState(float waitTime)
{
	if(state() != EnemyState::Dead)
		mStateMachine.addState(new EnemyWait(this, waitTime));
}


void Enemy::addIdleState(float waitTime)
{
	if (state() != EnemyState::Dead)
		mStateMachine.addState(new EnemyIdle(this, waitTime));
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
