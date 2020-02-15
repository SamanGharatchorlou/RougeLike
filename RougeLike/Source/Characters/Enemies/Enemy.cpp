#include "pch.h"
#include "Enemy.h"

#include "Game/GameData.h"
#include "Game/Camera.h"

#include "System/Files/AnimationReader.h"
#include "System/Files/StatReader.h"


#include "EnemyEnums.h"
#include "EnemyPropertyBag.h"
#include "EnemyStates/EnemyStateHeaders.h"

#include "Characters/Player/Player.h"


Enemy::Enemy(GameData* gameData) : 
	mGameData(gameData),
	bag(nullptr),
	mIsActive(false)
{ }

Enemy::~Enemy() 
{
	delete bag;
	bag = nullptr;
}


void Enemy::init(std::string name)
{
	mFlip = SDL_FLIP_NONE;

	initAnimations(name);
	readAttributes(name);

	// Size
	VectorF size = mAnimator.getSpriteTile()->getRect().Size() * 2.0f;
	mRect = RectF(VectorF(), size);

	// Movement
	mMovement.setPosition(mRect.TopLeft());

	mCollider.init(&mRect);
	mMovement.init(&mCollider, bag->pMovementSpeed.get());
}

void Enemy::slowUpdate(float dt)
{
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

	mAnimator.getSpriteTile()->render(rect, mFlip);

#if DRAW_ENEMY_RECT
	debugDrawRect(mGameData, mRect, RenderColour(RenderColour::RED));
#endif
}


// enemy target - currently always the player
const RectF Enemy::targetRect() const
{
	return mGameData->player->getRect();
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
		break;

	case EnemyState::Hit:
		mStateMachine.addState(new EnemyHit(this));
		break;	
	
	case EnemyState::Patrol:
		mStateMachine.addState(new EnemyPatrol(this));
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


// -- Property Functions -- //
Health&		Enemy::health() const			{ return bag->pHealth.get(); }
bool		Enemy::isDead() const			{ return bag->pHealth.get().isDead(); }
						    
Damage&		Enemy::damage() const			{ return bag->pDamage.get(); }

float		Enemy::hurtTime() const			{ return bag->pHurtTime.get(); }
float		Enemy::chaseRange() const		{ return bag->pChaseRange.get(); }
float		Enemy::sightRange() const		{ return bag->pSightRange.get(); }
float		Enemy::attentionTime() const	{ return bag->pAttentionTime.get(); }
float		Enemy::movementSpeed() const	{ return bag->pMovementSpeed.get(); }
float		Enemy::tackleSpeed() const		{ return bag->pTackleSpeed.get(); }
float		Enemy::tackleDistance() const	{ return bag->pTackleDistance.get(); }
float		Enemy::tackleChargeTime() const { return bag->pTackleChargeTime.get(); }
int			Enemy::score() const			{ return bag->pScore.get(); }


// -- Private Functions -- //
void Enemy::readAttributes(std::string name)
{
	// read properties
	bag = new EnemyPropertyBag();

	StatReader statReader;
	XMLValueMap map = statReader.getStats(name);

	// Health
	Health health(map.getFloat("Health"));
	bag->pHealth.set(health);

	// Movement
	float movementSpeed = map.getFloat("MovementSpeed");
	bag->pMovementSpeed.set(movementSpeed);

	// Damage
	Damage damage(map.getFloat("AttackDamage"));
	bag->pDamage.set(damage);

	// Tackle Speed
	float tackleSpeed = map.getFloat("TackleSpeed");
	bag->pTackleSpeed.set(tackleSpeed);

	// Tackle Distance
	float tackleDistance = map.getFloat("TackleDistance");
	bag->pTackleDistance.set(tackleDistance);

	// Tackle Charge Time
	float tackleChargeTime = map.getFloat("TackleChargeTime");
	bag->pTackleChargeTime.set(tackleChargeTime);

	// Sight Range
	float sightRange = map.getFloat("SightRange");
	bag->pSightRange.set(sightRange);

	// Chase Range
	float chaseRange = map.getFloat("ChaseRange");
	bag->pChaseRange.set(chaseRange);

	// Hurt time
	float hurtTime = map.getFloat("HurtTime");
	bag->pHurtTime.set(hurtTime);

	// Attention Time
	float attentionTime = map.getFloat("AttentionTime");
	bag->pAttentionTime.set(attentionTime);

	// Score
	int score = map.getInt("Score");
	bag->pScore.set(score);
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
