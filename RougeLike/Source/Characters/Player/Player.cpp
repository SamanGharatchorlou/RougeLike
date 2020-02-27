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
	mFlip(SDL_FLIP_NONE),
	mState(PlayerState::None)
{ }


void Player::init(const std::string& characterConfig)
{
	// Setup stats
	propertyBag().readAttributes(characterConfig);

	// init physics
	physics.init(bag.pForce.get(), bag.pMaxVelocity.get(), bag.pDragFactor.get());

	// Setup animations
	initAnimations(characterConfig);

	// Size
	VectorF size = mAnimator.getSpriteTile()->getRect().Size() * 2;
	physics.setRect(RectF(VectorF(size.x * 2.0f, mGameData->map->size().y / 2.0f), size));

	VectorF colliderScale = VectorF(1.0f, 0.2f); // only with walls
	mCollider.init(&physics.getRect(), colliderScale);
}


void Player::processStateChanges()
{
	stateMachine.processStateChanges();
}


void Player::handleInput()
{
	InputManager& input = *mGameData->inputManager;

	physics.handleInput(&input);

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
	physics.update(dt);

	// Attack state
	stateMachine.getActiveState().fastUpdate(dt);
	
	if (physics.isMoving())
		mAnimator.setSpeedFactor(physics.relativeSpeed());
	else
		mAnimator.setSpeedFactor(1.0f);

	mAnimator.fastUpdate(dt);

	// Weapon
	mWeapon.fastUpdate(getRect().TopLeft());
}


void Player::render()
{	
	// Flip sprite
	if (mFlip == SDL_FLIP_NONE && physics.direction().x < 0)
	{
		mFlip = SDL_FLIP_HORIZONTAL;
	}
	else if (mFlip == SDL_FLIP_HORIZONTAL && physics.direction().x > 0)
	{
		mFlip = SDL_FLIP_NONE;
	}

#if DRAW_PLAYER_RECT
	debugDrawRect(mGameData, getRect(), RenderColour(RenderColour::GREEN));
	debugDrawRect(mGameData, mCollider.getRectBase(), RenderColour(RenderColour::BLUE));
#endif
#if DRAW_WEAPON_RECTS
	debugDrawRects(mGameData, mWeapon.getRects(), RenderColour(RenderColour::GREEN));
#endif

	// Character
	RectF rect = mGameData->camera->toCameraCoords(getRect());
	mAnimator.getSpriteTile()->render(rect, mFlip);

	// Weapon
	mWeapon.render();
}


void Player::updateState()
{
	PlayerState::actionState newState = physics.isMoving() ? PlayerState::Run : PlayerState::Idle;

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
void Player::initAnimations(const std::string& config)
{
	// config reader
	AnimationReader reader(config, mGameData->textureManager);

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