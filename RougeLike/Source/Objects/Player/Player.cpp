#include "pch.h"
#include "Player.h"

#include "Game/GameData.h"
#include "Game/Camera.h"
#include "Game/Cursor.h"
#include "Input/InputManager.h"
#include "Audio/AudioManager.h"
#include "Graphics/TextureManager.h"

#include "PlayerPropertyBag.h"
#include "Weapons/Weapon.h"
#include "States/State.h"

#include "Map/Environment.h"
#include "Map/Map.h"



Player::Player(GameData* gameData) : 
	Actor(gameData),
	mStateMachine(new NullState),
	mWeapon(nullptr)
{
#if _DEBUG
	mCollider.setName("player");
#endif
}


Player::~Player()
{
	// todo delete property bag
}

void Player::reset()
{
	propertyBag()->resetProperties();
	mStateMachine.clearStates();
}


void Player::init(const std::string& characterConfig)
{
	Actor::init(characterConfig);

	PlayerPropertyBag* propertyBag = new PlayerPropertyBag;
	propertyBag->readProperties(characterConfig);
	setPropertyBag(propertyBag);

	// Setup stats
	//propertyBag()->readAttributes(characterConfig);

	// init physics
	Physics::Data physicsData;
	physicsData.force = getPropertyValue("Force");
	physicsData.maxVelocity = getPropertyValue("MaxVelocity");
	mPhysics.init(physicsData);

	// rect tweak
	VectorF size = mAnimator.getSpriteTile()->getRect().Size() * 1.2f;
	mPhysics.setRect(RectF(VectorF(), size));

	VectorF colliderScale = VectorF(1.0f, 0.25f); // only with walls
	mCollider.init(&mPhysics.rectRef(), colliderScale);
}


void Player::handleInput()
{
	mPhysics.handleInput(mGameData->inputManager);

	if (mGameData->inputManager->isCursorPressed(Cursor::Left))
	{
		// Play miss sound
		if (!mWeapon->isAttacking())
		{
			mGameData->audioManager->playSound(mWeapon->missSoundLabel(), this);
		}

		mWeapon->attack();
	}
}


void Player::fastUpdate(float dt)
{
	Actor::fastUpdate(dt);

	//mPhysics.fastUpdate(dt);
	//mAnimator.fastUpdate(dt);
	mWeapon->fastUpdate(dt);
	
	//if (mPhysics.isMoving())
	//	mAnimator.setSpeedFactor(mPhysics.relativeSpeed());
	//else
	//	mAnimator.setSpeedFactor(1.0f);

	// Weapon
	mWeapon->updateAnchor(rect().TopLeft());
	mWeapon->updateAimDirection(mGameData->inputManager->cursorPosition());
}


void Player::slowUpdate(float dt)
{
	Actor::slowUpdate(dt);

	mWeapon->slowUpdate(dt);/*
	mAnimator.slowUpdate(dt);*/

	if (mWeapon->didHit())
	{
		// Play hit sound
		if (mGameData->audioManager->isPlaying(mWeapon->missSoundLabel(), this) && mWeapon->canPlayHitSound())
		{
			mGameData->audioManager->playSound(mWeapon->hitSoundLabel(), this);
		}
	}

	updateState();
}


void Player::render()
{	
	// Flip sprite
	if (mPhysics.flip() == SDL_FLIP_NONE && mPhysics.direction().x < 0)
	{
		mPhysics.setFlip(SDL_FLIP_HORIZONTAL);
	}
	else if (mPhysics.flip() == SDL_FLIP_HORIZONTAL && mPhysics.direction().x > 0)
	{
		mPhysics.setFlip(SDL_FLIP_NONE);
	}

#if DRAW_PLAYER_RECTS
	debugDrawRect(rect(), RenderColour(RenderColour::Green));
	debugDrawRect(mCollider.scaledRect(), RenderColour(RenderColour::Blue));
	debugDrawRects(mWeapon->getRects(), RenderColour(RenderColour::Yellow));
#else

	Actor::render();

	// Weapon
	mWeapon->render();
#endif
}


// Rather than decreasing the player rect, increase the size of the render object
RectF Player::renderRect() const
{
	RectF renderRect = rect();
	VectorF size = renderRect.Size();

	// scale render rect by 1.75
	renderRect.SetSize(size * 1.75f);

	VectorF sizeDiff = renderRect.Size() - size;

	renderRect = renderRect.Translate(sizeDiff.x / 2.0f * -1, sizeDiff.y * -1);

	return renderRect;
}


void Player::equiptWeapon(Weapon* weapon)
{
	mWeapon = weapon;
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

void Player::selectAnimation()
{
	if(mMoving)
		mAnimator.selectAnimation("Run");
	else
		mAnimator.selectAnimation("Idle");
}