#include "pch.h"
#include "Player.h"

#include "States/State.h"

#include "Game/GameData.h"
#include "Game/Cursor.h"
#include "Input/InputManager.h"
#include "Graphics/TextureManager.h"
#include "Game/Camera.h"

#include "PlayerPropertyBag.h"

#include "Weapons/Weapon.h"

#include "System/Files/AnimationReader.h"
#include "Map/MapLevel.h" // TODO: do i need both maplevel and map?
#include "Map/Map.h"


Player::Player(GameData* gameData) : 
	mGameData(gameData),
	mStateMachine(new NullState),
	mBag(new PlayerPropertyBag),
	mWeapon(nullptr),
	mFlip(SDL_FLIP_NONE)
{

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
	mPhysics.init(mBag->pForce.get(), mBag->pMaxVelocity.get(), mBag->pDragFactor.get());

	// Setup animations
	initAnimations(characterConfig);

	// Size

	// rect tweak
	VectorF size = mAnimator.getSpriteTile()->getRect().Size() * 1.2f;
	mPhysics.setRect(RectF(VectorF(), size));

	VectorF colliderScale = VectorF(1.0f, 0.25f); // only with walls
	mCollider.init(&mPhysics.rectRef(), colliderScale);
}


void Player::handleInput()
{
	mPhysics.handleInput(mGameData->inputManager);

	if (mGameData->inputManager->isCursorPressed())
	{
		mWeapon->attack();
	}
}


void Player::fastUpdate(float dt)
{
	mPhysics.update(dt);

	mWeapon->fastUpdate(dt);
	
	if (mPhysics.isMoving())
		mAnimator.setSpeedFactor(mPhysics.relativeSpeed());
	else
		mAnimator.setSpeedFactor(1.0f);

	mAnimator.fastUpdate(dt);

	// Weapon
	mWeapon->updateAnchor(rect().TopLeft());
	mWeapon->updateAimDirection(mGameData->cursor->getPosition());
}


void Player::slowUpdate(float dt)
{
	mWeapon->slowUpdate(dt);

	mAnimator.slowUpdate(dt);

	updateState();
}


void Player::render()
{	
	// Flip sprite
	if (mFlip == SDL_FLIP_NONE && mPhysics.direction().x < 0)
	{
		mFlip = SDL_FLIP_HORIZONTAL;
	}
	else if (mFlip == SDL_FLIP_HORIZONTAL && mPhysics.direction().x > 0)
	{
		mFlip = SDL_FLIP_NONE;
	}

#if DRAW_PLAYER_RECT
	debugDrawRect(rectA(), RenderColour(RenderColour::Green));
	debugDrawRect(mCollider.getRectBase(), RenderColour(RenderColour::Blue));
	debugDrawRects(mWeapon->getRects(), RenderColour(RenderColour::Yellow));
#else
	// Character
	RectF rect = Camera::Get()->toCameraCoords(renderRect());
	mAnimator.getSpriteTile()->render(rect, mFlip);

	// Weapon
	mWeapon->render();
#endif
}


// Rather than decreasing the player rect, increase the size of the render object
RectF Player::renderRect() const
{
	RectF renderRect = rect();
	VectorF size = renderRect.Size();

	renderRect.SetSize(size * 1.75f);

	VectorF sizeDiff = renderRect.Size() - size;

	renderRect = renderRect.Translate(sizeDiff.x / 2.0f * -1, sizeDiff.y * -1);

	return renderRect;
}


void Player::equiptWeapon(Weapon* weapon)
{
	mWeapon = weapon;
	updateWeaponStats(mBag);
}


void Player::updateWeaponStats(const PlayerPropertyBag* bag)
{
	mWeapon->updateStats(bag);
}


void Player::updateState()
{
	bool isMoving = mPhysics.isMoving();

	if (mMoving != mPhysics.isMoving())
	{
		mMoving = mPhysics.isMoving();
		selectAnimation();
	}
}



// -- Private Functions -- //

void Player::initAnimations(const std::string& config)
{
	// config reader
	AnimationReader reader(config);

	// Setup sprite sheet
	TilesetData spriteSheetData = reader.readTilesetData(mGameData->textureManager);
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