#include "pch.h"
#include "Player.h"

#include "Game/Camera/Camera.h"
#include "Input/InputManager.h"
#include "Audio/AudioManager.h"

#include "Map/Map.h"
#include "Weapons/Weapon.h"
#include "Weapons/Melee/MeleeWeapon.h"

#include "PlayerStates/PlayerAttackState.h"
#include "PlayerStates/PlayerDeadState.h"

#if DRAW_PLAYER_RECTS
#include "Debug/DebugDraw.h"
#endif



Player::Player() :
	mWeapon(nullptr),
	mMapLevel(0),
	mControlOverride(false),
	mStateMachine(new PlayerNullState)
{ }


void Player::handleInput(const InputManager* input)
{
	mPhysics.handleInput(input);
	updateCursorPosition(input->cursorPosition());
}


void Player::move(float dt)
{
	if (userHasControl())
	{
		mPhysics.move(dt);
	}
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
	handleStates(dt);
	updateMapInfo();

	AudioManager* audio = AudioManager::Get();
	Health* health = static_cast<Health*>(getAttribute(AttributeType::Health));

	if (!health->isDead())
	{
		setMovementAnimation();
		updateMovementSound(audio);
	}

	handleHealthChanges(health);

	if(!health->isDead() && mCollider.gotHit())
	{
		handleHit(audio);
	}
}


void Player::clear()
{
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

		RectF renderRect = Camera::Get()->toCameraCoords(rect());
		const float flashTime = 0.1f;
		if (mColourModTimer.isStarted() && mColourModTimer.getSeconds() < flashTime)
		{
			RenderColour colourMod = RenderColour(225, 0, 0);
			mAnimator.render(renderRect, mPhysics.flip(), colourMod);
		}
		else
		{
			mColourModTimer.stop();
			mAnimator.render(renderRect, mPhysics.flip());
		}

		mEffects.render();
	}
}


void Player::overrideControl(bool control)
{
	mControlOverride = control;
}


// --- Private Functions --- //

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


void Player::resetColliders()
{
	mCollider.reset();
	weapon()->getCollider()->reset();
}


void Player::updateMapInfo()
{
	const Map* map = currentMap();

#if DEBUG_CHECK // break points might break this
	if (!map)
		return;
#endif

	int level = map->level();

	if (mMapLevel != level)
	{
		mMapLevel = level;
		MapLevelEvent* mapLevelEventPtr = new MapLevelEvent(mMapLevel);
		mEvents.push(EventPacket(mapLevelEventPtr));
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


void Player::handleStates(float dt)
{
	mStateMachine.getActiveState().slowUpdate(dt);

	if (mStateMachine.getActiveState().finished())
		mStateMachine.popState();

	PlayerState* usedState = mStateMachine.processStateChanges();
	if (usedState)
		delete usedState;
}


void Player::setMovementAnimation()
{
	Action action = mPhysics.isMoving() ? Action::Run : Action::Idle;
	mAnimator.selectAnimation(action);
}


void Player::updateMovementSound(AudioManager* audio)
{
	Action currentAction = mAnimator.currentAction();
	if (currentAction == Action::Run)
	{
		if (!audio->isPlaying("PlayerWalk2", this))
			audio->loopSoundGroup("PlayerWalk2", this);
	}
	else if (currentAction == Action::Idle)
	{
		if (audio->isPlaying("PlayerWalk2", this))
			audio->stop("PlayerWalk2", this);
	}
}


void Player::handleHit(AudioManager* audio)
{
	audio->playSound("PlayerHurt", this);

	TraumaEvent* trauma = new TraumaEvent(60);
	mEvents.push(EventPacket(trauma));

	mColourModTimer.restart();
}


void Player::handleHealthChanges(Health* health)
{
	if (health->hasChanged())
	{
		SetUISlider* eventPtr = new SetUISlider("HealthSlider", health->getPercentage());
		EventPacket event(eventPtr);
		mEvents.push(event);

		if (health->isDead() && !mControlOverride)
		{
			mStateMachine.addState(new PlayerDeadState(this));
			AudioManager::Get()->stop("PlayerWalk2", this);
		}

		health->changedHandled();
	}
}
