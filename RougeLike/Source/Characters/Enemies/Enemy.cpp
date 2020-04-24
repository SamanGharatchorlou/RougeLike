#include "pch.h"
#include "Enemy.h"
#include "EnemyStates/EnemyStateHeaders.h"

#include "Game/GameData.h"
#include "Game/Camera.h"

#include "Graphics/Texture.h"
#include "System/Files/AnimationReader.h"

#include "Characters/Player/Player.h"


Enemy::Enemy(GameData* gameData, AIPathMap* map) :
	mGameData(gameData),
	mMap(map),
	mStateMachine(new EnemyNullState),
	mAttackTarget(nullptr),
	mPositionTarget(nullptr)
{
	setFlip(static_cast<SDL_RendererFlip>(randomNumberBetween(0, 2)));
#if _DEBUG
	mCollider.setName("enemy");
#endif
}


void Enemy::init()
{
	Physics::Data physicsData;
	physicsData.force = mBag.pForce.get();
	physicsData.maxVelocity = mBag.pMaxVelocity.get();
	physicsData.dragFactor = mBag.pDragFactor.get();
	physicsData.mass = mBag.pMass.get();

	mPhysics.init(physicsData);
}


void Enemy::fastUpdate(float dt)
{
	mPhysics.resetHasForce();

	mStateMachine.getActiveState().fastUpdate(dt);
	mPhysics.fastUpdate(dt);
	mAnimator.fastUpdate(dt);
}


void Enemy::slowUpdate(float dt)
{
	// Reset alpha for enemies sharing the same texture
	mAnimator.getSpriteTexture()->setAlpha(alphaMax);

	mStateMachine.processStateChanges();

	mStateMachine.getActiveState().slowUpdate(dt);
	mAnimator.slowUpdate(dt);

	// reset collider data for next frame
	mCollider.reset();
}


void Enemy::render()
{
	mStateMachine.getActiveState().render();
}


void Enemy::renderCharacter()
{
	RectF rect = renderRect();
	rect = Camera::Get()->toCameraCoords(rect);

#if DRAW_ENEMY_RECT
	debugDrawRect(mRect, RenderColour(RenderColour::Red));
#else
	mAnimator.getSpriteTile()->render(rect, mFlip);
#endif
}



void Enemy::clear()
{
	// Clear statemachine states (except null state at i = 0)
	while (mStateMachine.size() > 1)
	{
		mStateMachine.popState();
		mStateMachine.processStateChanges();
	}

	while (events.size() > 0)
		events.pop();

	mFlip = SDL_FLIP_NONE;
	mAttackTarget = nullptr;
	mPositionTarget = nullptr;
}


void Enemy::resolvePlayerWeaponCollisions()
{
	if (mCollider.gotHit())
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

void Enemy::facePoint(VectorF point)
{
	if (point.x > position().x) // -->
		mFlip = SDL_FLIP_NONE;
	else // <--
		mFlip = SDL_FLIP_HORIZONTAL;
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
			
	case EnemyState::PreAttack:
		mStateMachine.addState(new EnemyPreAttack(this));
		break;

	case EnemyState::Hit:
		mStateMachine.addState(new EnemyHit(this));
		break;	
	
	case EnemyState::Patrol:
		mStateMachine.addState(new EnemyPatrol(this));
		break;

	case EnemyState::None:
		mStateMachine.addState(new EnemyNullState());
		break;

	case EnemyState::Idle:
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


void Enemy::initAnimations(std::string file)
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