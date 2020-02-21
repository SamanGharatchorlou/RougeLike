#include "pch.h"
#include "Enemy.h"

#include "Game/GameData.h"
#include "Game/Camera.h"

#include "System/Files/AnimationReader.h"

#include "Graphics/Texture.h"

#include "EnemyEnums.h"
#include "EnemyPropertyBag.h"
#include "EnemyStates/EnemyStateHeaders.h"

#include "Characters/Player/Player.h"


Enemy::Enemy(GameData* gameData) : 
	mGameData(gameData),
	mIsActive(false),
	mAlpha(alphaMax)
{ }


void Enemy::init(std::string name)
{
	mFlip = SDL_FLIP_NONE;

	initAnimations(name);
	bag.readAttributes(name);

	// Size
	VectorF size = mAnimator.getSpriteTile()->getRect().Size() * 2.0f;
	mRect = RectF(VectorF(), size);

	// Movement
	mMovement.setPosition(mRect.TopLeft());

	mCollider.init(&mRect);
	mMovement.init(&mCollider, bag.pMovementSpeed.get());
}

void Enemy::slowUpdate(float dt)
{
	// reset alpha for enemies sharing the same texture
	mAnimator.getSpriteTexture()->setAlpha(alphaMax);

	mStateMachine.processStateChanges();

	mStateMachine.getActiveState().slowUpdate(dt);

	mAnimator.slowUpdate(dt);
}


void Enemy::fastUpdate(float dt)
{
	mStateMachine.getActiveState().fastUpdate(dt);

	mAnimator.fastUpdate(dt);
}


void Enemy::render()
{
	// Flip sprite
	if (mFlip == SDL_FLIP_NONE && mMovement.getDirection().x < 0)
	{
		mFlip = SDL_FLIP_HORIZONTAL;
	}
	else if (mFlip == SDL_FLIP_HORIZONTAL && mMovement.getDirection().x >= 0)
	{
		mFlip = SDL_FLIP_NONE;
	}

	// render in camera coords
	VectorF cameraPosition = mGameData->camera->toCameraCoords(mMovement.getPostion());
	RectF rect = RectF(cameraPosition, mRect.Size());

	mAnimator.getSpriteTexture()->setAlpha(mAlpha);
	mAnimator.getSpriteTile()->render(rect, mFlip);

#if DRAW_ENEMY_RECT
	debugDrawRect(mGameData, mRect, RenderColour(RenderColour::RED));
#endif
}


void Enemy::spawn(VectorF position)
{
	addState(EnemyState::Patrol);

	mMovement.setPosition(position);
}

void Enemy::move(float dt)
{
	mMovement.fastUpdate(dt);
	mRect.SetTopLeft(mMovement.getPostion());
}


void Enemy::replaceState(EnemyState state)
{
	switch (state)
	{
	case EnemyState::Idle:
		mStateMachine.replaceState(new EnemyIdle(this));

		mState.pop();
		mState.push(EnemyState::Idle);
		break;

	case EnemyState::Run:
		mStateMachine.replaceState(new EnemyRun(this));

		mState.pop();
		mState.push(EnemyState::Run);
		break;

	case EnemyState::Patrol:
		mStateMachine.replaceState(new EnemyPatrol(this));

		mState.pop();
		mState.push(EnemyState::Patrol);
		break;

	case EnemyState::Alert:
		mStateMachine.replaceState(new EnemyAlert(this));

		mState.pop();
		mState.push(EnemyState::Alert);
		break;

	case EnemyState::Hit:
		mStateMachine.replaceState(new EnemyHit(this));

		mState.pop();
		mState.push(EnemyState::Hit);
		break;

	case EnemyState::Dead:
		mStateMachine.replaceState(new EnemyDead(this));

		mState.pop();
		mState.push(EnemyState::Dead);
		break;

	case EnemyState::Attack:
	case EnemyState::None:
	default:
		DebugPrint(Warning, "No enemy state set, no state was replaced\n");
		break;
	}
}

void Enemy::addState(EnemyState state)
{
	switch (state)
	{
	case EnemyState::Attack:
		mStateMachine.addState(new EnemyAttack(this));
		mState.push(EnemyState::Attack);
		break;

	case EnemyState::Hit:
		mStateMachine.addState(new EnemyHit(this));
		mState.push(EnemyState::Hit);
		break;	
	
	case EnemyState::Patrol:
		mStateMachine.addState(new EnemyPatrol(this));
		mState.push(EnemyState::Patrol);
		break;

	case EnemyState::None:
	case EnemyState::Idle:
	case EnemyState::Run:
	case EnemyState::Alert:
	case EnemyState::Dead:
	default:
		DebugPrint(Warning, "No enemy state set, no state was added\n");
		break;
	}
}


void Enemy::initAnimations(std::string file)
{
	// config reader
	AnimationReader reader(file, mGameData->textureManager);;

	// Setup sprite sheet
	TilesetData spriteSheetData = reader.readTilesetData();
	Tileset spriteSheet(spriteSheetData);

	// Setup animations
	Animations animationData = reader.readAnimationData();
	mAnimator.init(spriteSheet, animationData);
}
