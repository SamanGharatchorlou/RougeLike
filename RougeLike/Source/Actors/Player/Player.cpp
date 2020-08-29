#include "pch.h"
#include "Player.h"

#include "Game/Camera/Camera.h"
#include "Input/InputManager.h"
#include "Audio/AudioManager.h"

#include "Map/Map.h"
#include "Weapons/Weapon.h"
#include "Weapons/Melee/MeleeWeapon.h"

#include "PlayerStates/PlayerAttackState.h"

#if DRAW_PLAYER_RECTS
#include "Debug/DebugDraw.h"
#endif



Player::Player() :
	mWeapon(nullptr),
	mMapLevel(0),
	mControlOverride(false),
	mStateMachine(new PlayerNullState)
{ }


void Player::setCharacter(const XMLNode playerNode)
{
	Actor::setCharacter(playerNode);
}


void Player::handleInput(const InputManager* input)
{
	mPhysics.handleInput(input);
	updateCursorPosition(input->cursorPosition());
}



void Player::move(float dt)
{
	if(userHasControl())
		mPhysics.move(dt);
}


void Player::fastUpdate(float dt)
{
	Actor::fastUpdate(dt);

	mWeapon->setPosition(rect().Center());
	mStateMachine.getActiveState().fastUpdate(dt);
 }


void Player::slowUpdate(float dt)
{
	Actor::slowUpdate(dt);

	mStateMachine.getActiveState().slowUpdate(dt);

	if (mStateMachine.getActiveState().finished())
		mStateMachine.popState();

	mStateMachine.processStateChanges();

	Action action = mPhysics.isMoving() ? Action::Run : Action::Idle;
	mAnimator.selectAnimation(action);

	//if (collider()->gotHit())
	//	processHit();
}


void Player::clear()
{
	mTileIndex.zero();
	mWeapon = nullptr;
	mControlOverride = false;
	Actor::clear();
}


void Player::setWeaponType(Weapon* weapon) 
{ 
	mWeapon = weapon; 
}


void Player::selectWeapon(WeaponData* weaponData)
{
	mWeapon->equipt(weaponData);
}


MeleeWeapon* Player::weapon()
{
	return static_cast<MeleeWeapon*>(mWeapon);
}


void Player::render()
{
#if DRAW_PLAYER_RECT
	debugDrawRect(rect(), RenderColour(RenderColour::Green));
#endif
#if TRACK_COLLISIONS
	collider()->renderCollider();
#endif

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
		handleEffects(effectCollider);

		//TraumaEvent* trauma = new TraumaEvent(40);
		//mEvents.push(EventPacket(trauma));
	}
}


bool Player::isAttacking() const
{
	return mWeapon->isAttacking();
}

void Player::attack()
{
	mStateMachine.addState(new PlayerAttackState(this));
}

Collider* Player::attackingCollider()
{
	return weapon()->getCollider();
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


void Player::updateMapInfo()
{
	const Map* map = currentMap();

#if IGNORE_WALLS // We might want to walk outside of the map
	if (!map)
		return;
#endif

	if(map->isValidPosition(position()))
	{
		Vector2D<int> currentTile = map->index(position());
		if (mTileIndex != currentTile)
		{
			mTileIndex = currentTile;

			UpdateAIPathMapEvent* eventPtr = new UpdateAIPathMapEvent;
			mEvents.push(EventPacket(eventPtr));
		}
	}

	int level = map->level();

	if (mMapLevel != level)
	{
		mMapLevel = level;
		UpdateTextBoxEvent* eventPtr = new UpdateTextBoxEvent("MapLevel val", BasicString(mMapLevel));
		mEvents.push(EventPacket(eventPtr));
	}
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
