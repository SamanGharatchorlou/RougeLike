
#include "pch.h"
#include "Player.h"

#include "Game/GameData.h"
#include "Game/Cursor.h"
#include "Game/Camera.h"
#include "Input/InputManager.h"
#include "Audio/AudioManager.h"
#include "UI/UIManager.h"

#include "Collisions/CollisionManager.h"
#include "Collisions/EffectCollider.h"
#include "Objects/Effects/EffectPool.h"
#include "Objects/Effects/EffectTypes/DamageEffect.h"
#include "Objects/Effects/EffectTypes/DisplacementEffect.h"

#include "Weapons/Melee/MeleeWeapon.h"

#include "Map/Environment.h"
#include "Map/Map.h"

#if _DEBUG
#include "Debug/DebugDraw.h"
#endif



Player::Player(GameData* gameData) :
	Actor(gameData),
	mAbilities(mGameData, this),
	mWeapon(nullptr),
	mControlOverride(false)
{ }


void Player::init(const std::string& characterConfig)
{
	Actor::init(characterConfig);
}


void Player::addAbility(const std::string& name)
{
	mAbilities.add(name);
}


void Player::handleInput(InputManager* input)
{
	mPhysics.handleInput(input);
	mAbilities.handleInput();
}

void Player::updateCursorPosition(VectorF cursorPosition)
{
	VectorF playerPosition = Camera::Get()->toCameraCoords(position());

	if (cursorPosition.x > playerPosition.x)
	{
		mPhysics.setFlip(SDL_FLIP_NONE);
		mWeapon->rightFlip();
	}
	else
	{
		mPhysics.setFlip(SDL_FLIP_HORIZONTAL);
		mWeapon->leftFlip();
	}

	mWeapon->updateAimDirection(cursorPosition);
}


void Player::fastUpdate(float dt)
{
	//mCollisionManager.fastUpdate(dt, mGameData->environment->map(position()));

	// Movement, animations, weapon updates
	Actor::fastUpdate(dt);

	//VectorF cursorPosition = mGameData->inputManager->cursorPosition();
	//VectorF playerPosition = Camera::Get()->toCameraCoords(position());

	//if (cursorPosition.x > playerPosition.x)
	//{
	//	mPhysics.setFlip(SDL_FLIP_NONE);
	//	mWeapon->rightFlip();
	//}
	//else
	//{
	//	mPhysics.setFlip(SDL_FLIP_HORIZONTAL);
	//	mWeapon->leftFlip();
	//}

	mWeapon->setPosition(rect().Center());
	mWeapon->fastUpdate(dt);

	//mWeapon->updateAimDirection(cursorPosition);
 }


void Player::slowUpdate(float dt)
{
	// Actor
	Actor::slowUpdate(dt);

	//mCollisionManager.slowUpdate(mGameData->environment->map(position()));

	// Weapon
	mWeapon->slowUpdate(dt);
	//if (weapon()->isAttacking())
	//{
	//	updateWeaponHitSound();
	//}

	// Abilities
	mAbilities.slowUpdate(dt);
	if (mAbilities.hasEvent())
		pushEvent(mAbilities.popEvent());

	// Character
	Action action = mPhysics.isMoving() ? Action::Run : Action::Idle;
	mAnimator.selectAnimation(action);

	if (collider()->gotHit())
		processHit();
/*
	updateCurrentTile();*/
}


void Player::reset()
{
	tileIndex.zero();
	Actor::reset();
}


void Player::selectCharacter(const std::string& character)
{
	init(character);
}


void Player::setWeapon(MeleeWeapon* weapon)
{
	mWeapon = weapon;

	// TODO: can i remove this?
	if (mPhysics.flip() == SDL_FLIP_HORIZONTAL)
	{
		mWeapon->leftFlip();
	}
	else if (mPhysics.flip() == SDL_FLIP_NONE)
	{
		mWeapon->rightFlip();
	}
}


MeleeWeapon* Player::weapon()
{
	return mWeapon;
}


void Player::render()
{
#if DRAW_PLAYER_RECTS
	debugDrawRect(rect(), RenderColour(RenderColour::Green));
	debugDrawRect(mCollider.scaledRect(), RenderColour(RenderColour::Blue));
	debugDrawRects(mWeapon->getRects(), RenderColour(RenderColour::Yellow));
#endif

	mAbilities.render();

	if (mVisibility)
	{
		mWeapon->render();
		Actor::render();
	}
}


void Player::overrideControl(bool control)
{
	mControlOverride = control;
	
	if (mWeapon)
		mWeapon->overrideCursorControl(!control);
}


// --- Private Functions --- //

void Player::processHit()
{
	if (mCollider.getOtherCollider())
	{
		EffectCollider* effectCollider = static_cast<EffectCollider*>(mCollider.getOtherCollider());
		processEffects(effectCollider);

		if (effectCollider->effectCount() == 0)
			printf("zero effects?\n");


		TraumaEvent* trauma = new TraumaEvent(40);
		pushEvent(EventPacket(trauma));
	}

	updateUI();
}


void Player::updateUI()
{
	// Update UI
	Health* hp = static_cast<Health*>(getProperty("Health"));
	SetHealthBarEvent* hpPtr = new SetHealthBarEvent(*hp);
	pushEvent(EventPacket(hpPtr));

	Armor* armor = static_cast<Armor*>(getProperty("Armor"));
	SetArmorBarEvent* armorPtr = new SetArmorBarEvent(*armor);
	pushEvent(EventPacket(armorPtr));
}


bool Player::attemptAttack()
{
	bool canAttack = !mAbilities.inSelectionMode() && !mWeapon->isAttacking();

	if (canAttack)
	{
		mWeapon->attack();
	}

	return canAttack;
}


void Player::updateWeaponHitSound(AudioManager* audio)
{
	if (mWeapon->didHit())
	{
		if (audio->isPlaying(mWeapon->missSoundLabel(), this) && mWeapon->canPlayHitSound())
		{
			audio->playSound(mWeapon->hitSoundLabel(), this);
		}
	}
}


void Player::updateCurrentTile(Map* map)
{
	//// Update enemy paths when player changes tile
	//Map* currentMap = mGameData->environment->map(position());

	if (map->isValidPosition(position()))
	{
		Vector2D<int> currentTile = map->index(position());
		if (tileIndex != currentTile)
		{
			tileIndex = currentTile;

			UpdateAIPathMapEvent* eventPtr = new UpdateAIPathMapEvent;
			pushEvent(EventPacket(eventPtr));
		}
	}
}


void Player::enableBodyCollisions(bool isEnabled)
{
	mBodyCollisions = isEnabled;
}
