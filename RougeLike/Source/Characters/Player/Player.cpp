#include "pch.h"
#include "Player.h"

#include "Game/GameData.h"
#include "Game/Cursor.h"
#include "Input/InputManager.h"
#include "Graphics/TextureManager.h"
#include "Game/Camera.h"


#include "PlayerPropertyBag.h"

#include "Weapons/Weapon.h"
#include "States/MeleeAttackState.h"

#include "System/Files/AnimationReader.h"
#include "Map/Map.h"


Player::Player(GameData* gameData) : 
	mGameData(gameData),
	mFlip(SDL_FLIP_NONE)
{
	mBag = new PlayerPropertyBag;
}


Player::~Player()
{
	delete mBag;
	mBag = nullptr;
}


void Player::init(const std::string& characterConfig)
{
	// Setup stats
	propertyBag()->readAttributes(characterConfig);

	// init physics
	physics.init(mBag->pForce.get(), mBag->pMaxVelocity.get(), mBag->pDragFactor.get());

	// Setup animations
	initAnimations(characterConfig);

	// Size
	VectorF size = mAnimator.getSpriteTile()->getRect().Size() * 2;
	physics.setRect(RectF(VectorF(size.x * 2.0f, mGameData->map->size().y / 2.0f), size));

	VectorF colliderScale = VectorF(1.0f, 0.2f); // only with walls
	mCollider.init(&physics.getRect(), colliderScale);
}


void Player::handleInput()
{
	physics.handleInput(mGameData->inputManager);

	if (mGameData->inputManager->isCursorPressed())
	{
		mWeapon->attack();
	}
}


void Player::slowUpdate(float dt)
{
	mAnimator.slowUpdate(dt);

	updateState();
}


void Player::fastUpdate(float dt)
{
	physics.update(dt);

	mWeapon->fastUpdate(dt);
	
	if (physics.isMoving())
		mAnimator.setSpeedFactor(physics.relativeSpeed());
	else
		mAnimator.setSpeedFactor(1.0f);

	mAnimator.fastUpdate(dt);

	// Weapon
	mWeapon->updateAnchor(getRect().TopLeft());
	mWeapon->updateAimDirection(mGameData->camera, mGameData->cursor->getPosition());
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
	mWeapon->render(mGameData->camera);
}


void Player::equiptWeapon(const WeaponData* data)
{
	mWeapon->equipt(data);
	updateWeaponStats(mBag);
}


void Player::updateWeaponStats(const PlayerPropertyBag* bag)
{
	mWeapon->updateStats(bag);
}


void Player::updateState()
{
	bool isMoving = physics.isMoving();

	if (mMoving != physics.isMoving())
	{
		mMoving = physics.isMoving();
		selectAnimation();
	}
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



void Player::selectAnimation()
{
	if(mMoving)
		mAnimator.selectAnimation("Run");
	else
		mAnimator.selectAnimation("Idle");
}