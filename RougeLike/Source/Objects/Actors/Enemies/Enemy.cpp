#include "pch.h"
#include "Enemy.h"

#include "Game/GameData.h"
#include "EnemyStates/EnemyStateHeaders.h"

#include "Graphics/Texture.h"
#include "System/Files/AnimationReader.h"

#include "EnemyPropertyBag.h"
#include "Map/Environment.h"
#include "Collisions/DamageCollider.h"

#include "Objects/Actors/ActorManager.h"
#include "Objects/Actors/Player/Player.h"
#include "Objects/Effects/KnockbackEffect.h"
#include "Objects/Effects/SlowEffect.h"



Enemy::Enemy(GameData* gameData) :
	Actor(gameData),
	mStateMachine(new EnemyNullState),
	mMap(nullptr),
	mAttackTarget(nullptr),
	mPositionTarget(nullptr)
{
	physics()->setFlip(static_cast<SDL_RendererFlip>(randomNumberBetween(0, 2)));
}


void Enemy::init(const std::string& characterConfig)
{
	// Property bag
	EnemyPropertyBag* propertyBag = new EnemyPropertyBag;
	propertyBag->readProperties(characterConfig);
	setPropertyBag(propertyBag);

	Actor::init(characterConfig);

	// Physics
	VectorF size = mAnimator.getSpriteTile()->getRect().Size() * 1.5f;
	mColliderRatio = VectorF(0.75f, 1.0f);
	mPhysics.setRect(RectF(VectorF(), size * mColliderRatio));

	// Collider
	DamageCollider* collider = new DamageCollider;
	collider->init(&mPhysics.rectRef());
	
	Damage* damage = static_cast<Damage*>(getProperty("Damage"));
	collider->initDamage(*damage, getPropertyValue("KnockbackDistance"));
	setCollider(collider);
#if _DEBUG
	mCollider->setName("enemy");
#endif
}


void Enemy::fastUpdate(float dt)
{
	mPhysics.resetHasForce();

	mStateMachine.getActiveState().fastUpdate(dt);

	Actor::fastUpdate(dt);
}


void Enemy::slowUpdate(float dt)
{
	Actor::slowUpdate(dt);
	// Reset alpha for enemies sharing the same texture
	mAnimator.getSpriteTexture()->setAlpha(alphaMax);

	mStateMachine.processStateChanges();
	mStateMachine.getActiveState().slowUpdate(dt);
}


void Enemy::render()
{
	mStateMachine.getActiveState().render();
}


void Enemy::renderCharacter()
{
	Actor::render();

#if DRAW_ENEMY_RECT
	debugDrawRect(mRect, RenderColour(RenderColour::Red));
#endif
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

	while (mEvents.size() > 0)
		mEvents.pop();

	mMap = nullptr;
	mAttackTarget = nullptr;
	mPositionTarget = nullptr;

	Actor::reset();
}


void Enemy::resolvePlayerWeaponCollisions()
{
	if (mCollider->gotHit())
	{
		// Apply knockback
		const DamageCollider* collider = static_cast<const DamageCollider*>(mCollider->getOtherCollider());
		KnockbackEffect* effect = new KnockbackEffect(mGameData->actors->player()->position(), collider->knockbackforce());
		mEffects.addEffect(effect);

		replaceState(EnemyState::Hit);
	}
}


void Enemy::spawn(EnemyState::Type state, VectorF position)
{
	addState(state);
	mPhysics.setPosition(position);
}


void Enemy::accellerateTowards(VectorF position)
{
	mPhysics.accellerate(position - mPhysics.position());
}


void Enemy::replaceState(EnemyState::Type state)
{
	switch (state)
	{
	case EnemyState::Idle:
		mStateMachine.replaceState(new EnemyIdle(this));
		break;

	case EnemyState::Run:
		mStateMachine.replaceState(new EnemyRun(this));
		break;

	case EnemyState::Patrol:
		mStateMachine.replaceState(new EnemyPatrol(this));
		break;

	case EnemyState::Alert:
		mStateMachine.replaceState(new EnemyAlert(this));
		break;

	case EnemyState::Hit:
		mStateMachine.replaceState(new EnemyHit(this));
		break;

	case EnemyState::Dead:
		mStateMachine.replaceState(new EnemyDead(this));
		break;

	case EnemyState::Attack:
		mStateMachine.replaceState(new EnemyAttack(this));
		break;

	case EnemyState::PreAttack:
	case EnemyState::None:
	default:
		DebugPrint(Warning, "No enemy state set, no state was replaced\n");
		break;
	}
}


void Enemy::addState(EnemyState::Type state)
{
	switch (state)
	{
	case EnemyState::Wait:
		mStateMachine.addState(new EnemyWait(this));
		break;
			
	case EnemyState::PreAttack:
		mStateMachine.addState(new EnemyPreAttack(this));
		break;

	case EnemyState::Hit:
		mStateMachine.addState(new EnemyHit(this));
		break;	
	
	case EnemyState::Patrol:
		mStateMachine.addState(new EnemyPatrol(this));
		break;

	case EnemyState::Idle:
		mStateMachine.addState(new EnemyIdle(this));
		break;

	case EnemyState::None:
		mStateMachine.addState(new EnemyNullState());
		break;

	case EnemyState::Run:
	case EnemyState::Alert:
	case EnemyState::Dead:
	default:
		DebugPrint(Warning, "No enemy state set, no state was added\n");
		break;
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


const EventPacket Enemy::popEvent()
{
	const EventPacket event = mEvents.front();
	mEvents.pop();
	return event;
}