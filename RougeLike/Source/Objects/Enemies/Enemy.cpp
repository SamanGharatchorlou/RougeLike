#include "pch.h"
#include "Enemy.h"
#include "EnemyStates/EnemyStateHeaders.h"

#include "Game/GameData.h"
#include "Game/Camera.h"

#include "Graphics/Texture.h"
#include "System/Files/AnimationReader.h"

#include "Objects/Player/Player.h"



#include "Collisions/DamageCollider.h"


Enemy::Enemy(GameData* gameData) :
	Actor(gameData),
	mStateMachine(new EnemyNullState),
	mMap(nullptr),
	mAttackTarget(nullptr),
	mPositionTarget(nullptr)
{
	setFlip(static_cast<SDL_RendererFlip>(randomNumberBetween(0, 2)));
}


void Enemy::init(const std::string& characterConfig)
{
	Actor::init(characterConfig);

	// Property bag
	EnemyPropertyBag* propertyBag = new EnemyPropertyBag;
	propertyBag->readProperties(characterConfig);
	setPropertyBag(propertyBag);

	// Physics
	Physics::Data physicsData;
	physicsData.force = getPropertyValue("Force");
	physicsData.maxVelocity = getPropertyValue("MaxVelocity");
	mPhysics.init(physicsData);

	VectorF size = mAnimator.getSpriteTile()->getRect().Size() * 1.5f;
	colliderRatio = VectorF(0.75f, 1.0f);
	mPhysics.setRect(RectF(VectorF(), size * colliderRatio));


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


DamageCollider* Enemy::damageCollider() const
{
	return static_cast<DamageCollider*>(mCollider);
}


void Enemy::fastUpdate(float dt)
{
	mPhysics.resetHasForce();
	Actor::fastUpdate(dt);

	mStateMachine.getActiveState().fastUpdate(dt);
}


void Enemy::slowUpdate(float dt)
{
	Actor::slowUpdate(dt);
	// Reset alpha for enemies sharing the same texture
	mAnimator.getSpriteTexture()->setAlpha(alphaMax);

	mStateMachine.processStateChanges();
	mStateMachine.getActiveState().slowUpdate(dt);

	// TODO: do i need this?
	// reset collider data for next frame
	//mCollider.reset();
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



// Reset everything that needs to be recalculated when spawned
void Enemy::clear()
{
	// Clear statemachine states (except null state at i = 0)
	while (mStateMachine.size() > 1)
	{
		mStateMachine.forcePopState();
	}

	while (events.size() > 0)
		events.pop();

	
	mMap = nullptr;
	mPhysics.reset();

	mAttackTarget = nullptr;
	mPositionTarget = nullptr;
}


void Enemy::resolvePlayerWeaponCollisions()
{
	if (mCollider->gotHit())
		replaceState(EnemyState::Hit);
}


void Enemy::spawn(EnemyState::Type state, VectorF position)
{
	addState(state);
	mPhysics.setPosition(position);
}


void Enemy::accellerateTowards(VectorF position)
{
	VectorF direction = position - mPhysics.position();
	mPhysics.accellerate(direction); 
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


void Enemy::initAnimations(const std::string& file)
{
	// Config reader
	AnimationReader reader(file);

	// Setup sprite sheet
	TilesetData spriteSheetData = reader.readTilesetData(mGameData->textureManager);
	Tileset spriteSheet(spriteSheetData);

	// Setup animations
	Animations animationData = reader.readAnimationData();
	mAnimator.init(spriteSheet, animationData);
}


void Enemy::pushEvent(const EventPacket event)
{
	events.push(event);
}


const EventPacket Enemy::popEvent()
{
	const EventPacket event = events.front();
	events.pop();
	return event;
}