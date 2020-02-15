#include "pch.h"
#include "Player.h"

#include "Game/GameData.h"
#include "Input/InputManager.h"
#include "Graphics/TextureManager.h"
#include "Game/Camera.h"

#include "States/AttackState.h"

#include "System/Files/AnimationReader.h"

#include "Map/Map.h"

Player::Player(GameData* gameData) : 
	mGameData(gameData), 
	mWeapon(gameData),
	mFlip(SDL_FLIP_NONE)
{ 
	gameData->player = this;
}

Player::~Player() { }

void Player::init()
{
	// Setup animations
	initAnimations("Soldier.xml");

	mState = PlayerState::None;

	// Size
	Vector2D<int> sizeI = mAnimator.getSpriteTile()->getRect().Size() * 2;
	VectorF size(sizeI);

	mRect = RectF(VectorF(size.x * 2.0f, mGameData->map->size().y / 2.0f), size);

	VectorF colliderScale = VectorF(1.0f, 0.2f); // only with walls
	mCollider.init(&mRect, colliderScale);
	
	// Movement
	mMovement.init(&mCollider, 200.0f);
	mMovement.setPosition(mRect.TopLeft());

	// Weapon
	mWeapon.selectWeapon("Katana");
	mWeapon.setScale(1.5f);
}


void Player::processStateChanges()
{
	stateMachine.processStateChanges();
}


void Player::handleInput()
{
	InputManager& input = *mGameData->inputManager;
	mMovement.resetDirection();

	// Movement
	if (input.isHeld(Button::UP))
		mMovement.up();

	if (input.isHeld(Button::DOWN))
		mMovement.down();

	if (input.isHeld(Button::LEFT))
		mMovement.left();

	if (input.isHeld(Button::RIGHT))
		mMovement.right();

	// Attack
	if (input.isCursorPressed())
	{
		addState(PlayerState::Attack);
	}
}

void Player::slowUpdate(float dt)
{
	// Attack state
	stateMachine.getActiveState().slowUpdate(dt);

	mAnimator.slowUpdate(dt);

	updateState();
}


void Player::fastUpdate(float dt)
{
	// Position
	mMovement.fastUpdate(dt);
	mRect.SetTopLeft(mMovement.getPostion());

	// Attack state
	stateMachine.getActiveState().fastUpdate(dt);
	
	mAnimator.fastUpdate(dt);

	// Weapon
	mWeapon.fastUpdate(mMovement.getPostion());
}


void Player::resolveWallCollisions(float dt)
{
	RectF collisionRect = mCollider.getRectBase();

	bool restrictLeft = doesCollideLeft(collisionRect.TopLeft(), dt) || doesCollideLeft(collisionRect.BotLeft(), dt);
	bool restrictRight = doesCollideRight(collisionRect.TopRight(), dt) || doesCollideRight(collisionRect.BotRight(), dt);

	bool restrictUp = doesCollideTop(collisionRect.TopLeft(), dt) || doesCollideTop(collisionRect.TopRight(), dt);
	bool restrictDown = doesCollideBot(collisionRect.BotLeft(), dt) || doesCollideBot(collisionRect.BotRight(), dt);

	VectorF direction = mMovement.getDirection();

	if(restrictLeft || restrictRight)
		direction.x = 0.0f;

	if (restrictUp || restrictDown)
		direction.y = 0.0f;

	mMovement.setDirection(direction);
}



void Player::render()
{	
	// Flip sprite
	if (mFlip == SDL_FLIP_NONE && mMovement.getDirection().x < 0)
	{
		mFlip = SDL_FLIP_HORIZONTAL;
	}
	else if (mFlip == SDL_FLIP_HORIZONTAL && mMovement.getDirection().x > 0)
	{
		mFlip = SDL_FLIP_NONE;
	}

#if DRAW_PLAYER_RECT
	debugDrawRect(mGameData, mRect, RenderColour(RenderColour::GREEN));
	debugDrawRect(mGameData, mCollider.getRectBase(), RenderColour(RenderColour::BLUE));
#endif
#if DRAW_WEAPON_RECTS
	debugDrawRects(mGameData, mWeapon.getRects(), RenderColour(RenderColour::GREEN));
#endif

	// Character
	RectF rect = mGameData->camera->toCameraCoords(mRect);
	mAnimator.getSpriteTile()->render(rect, mFlip);

	// Weapon
	mWeapon.render();
}


void Player::updateState()
{
	PlayerState::actionState newState = mMovement.isMoving() ? PlayerState::Run : PlayerState::Idle;

	if (mState != newState)
	{
		selectAnimation(newState);
		mState = newState;
	}
}


void Player::addState(PlayerState::actionState newState)
{
	switch (newState)
	{
	case PlayerState::Attack:
		stateMachine.addState(new AttackState(mGameData, this));
		break;
	default:
		DebugPrint(Warning, "Failed adding %d player state\n", newState);
		break;
	}
}


void Player::popState()
{
	stateMachine.popState();
}


// -- Private Functions -- //
void Player::initAnimations(std::string config)
{
	// config reader
	AnimationReader reader(config, mGameData->textureManager);;

	// Setup sprite sheet
	TilesetData spriteSheetData = reader.readTilesetData();
	Tileset spriteSheet(spriteSheetData);

	// Setup animations
	Animations animationData = reader.readAnimationData();
	mAnimator.init(spriteSheet, animationData);
}



void Player::selectAnimation(PlayerState::actionState state)
{
	switch (state)
	{
	case PlayerState::Idle:
		mAnimator.selectAnimation("Idle");
		break;
	case PlayerState::Run:
		mAnimator.selectAnimation("Run");
		break;
	case PlayerState::Attack:
		break;
	default:
		DebugPrint(Warning, "Failed replacing player state with %d\n", state);
		break;
	}
}


bool Player::doesCollideLeft(VectorF point, float dt)
{
	MapTile currentTile = mGameData->map->getTile(point);
	ASSERT(Warning, currentTile.collisionType() == MapTile::Floor,
		"Player is not on a floor tile, tile at index %d,%d has a %d tile type", 
		currentTile.index.x, currentTile.index.y, currentTile.collisionType());

	// make sure left tile request is in bounds
	if (mGameData->map->getIndex(currentTile).x < 1)
		return false;

	MapTile leftTile = mGameData->map->offsetTile(currentTile, -1, 0);

	if (leftTile.hasCollisionType(MapTile::Right ^ MapTile::Wall))
	{
		float xFuturePosition = point.x + (mMovement.getMovementDistance().x * dt);
		return xFuturePosition < leftTile.rect().RightPoint();
	}

	return false;
}

bool Player::doesCollideRight(VectorF point, float dt)
{
	MapTile currentTile = mGameData->map->getTile(point);
	ASSERT(Warning, currentTile.collisionType() == MapTile::Floor,
		"Player is not on a floor tile, tile at index %d,%d has a %d tile type",
		currentTile.index.x, currentTile.index.y, currentTile.collisionType());

	// make sure right tile request is in bounds
	if (currentTile.rect().LeftPoint() >= mGameData->map->size().x)
		return false;

	MapTile rightTile = mGameData->map->offsetTile(currentTile, +1, 0);

	if (rightTile.hasCollisionType(MapTile::Left ^ MapTile::Wall))
	{
		float xFuturePosition = point.x + (mMovement.getMovementDistance().x * dt);
		return xFuturePosition > rightTile.rect().LeftPoint();
	}

	return false;
}

bool Player::doesCollideTop(VectorF point, float dt)
{
	MapTile currentTile = mGameData->map->getTile(point);	
	ASSERT(Warning, currentTile.collisionType() == MapTile::Floor,
		"Player is not on a floor tile, tile at index %d,%d has a %d tile type",
		currentTile.index.x, currentTile.index.y, currentTile.collisionType());

	MapTile upTile = mGameData->map->offsetTile(currentTile, 0, -1);

	if (upTile.hasCollisionType(MapTile::Bot))
	{
		float yFuturePosition = point.y + (mMovement.getMovementDistance().y * dt);
		return yFuturePosition < upTile.rect().BotPoint();
	}

	return false;
}

bool Player::doesCollideBot(VectorF point, float dt)
{
	MapTile currentTile = mGameData->map->getTile(point);
	ASSERT(Warning, currentTile.collisionType() == MapTile::Floor,
		"Player is not on a floor tile, tile at index %d,%d has a %d tile type",
		currentTile.index.x, currentTile.index.y, currentTile.collisionType());

	MapTile downTile = mGameData->map->offsetTile(currentTile, 0, +1);

	if (downTile.hasCollisionType(MapTile::Top))
	{
		float yFuturePosition = point.y + (mMovement.getMovementDistance().y * dt);
		return yFuturePosition > downTile.rect().TopPoint();
	}

	return false;
}


