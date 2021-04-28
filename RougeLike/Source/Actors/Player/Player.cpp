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

// TEMP
#include "Objects/Pools/EffectPool.h"

#if DRAW_PLAYER_RECTS
#include "Debug/DebugDraw.h"
#endif



Player::Player() :
	mWeapon(nullptr),
	mMapLevel(0),
	mControlOverride(false),
	mStateMachine(new PlayerNullState)
{
}

void Player::init()
{
	mStepTimer.start();

	mHP = static_cast<Health*>(getAttribute(AttributeType::Health));
	// Dont want this to trigger before the game screen
	mHP->changedHandled();
}


void Player::handleInput(const InputManager* input)
{
	mPhysics.handleInput(input);
	updateCursorPosition(input->cursorPosition());
}

void Player::attack()
{
	mWeapon->attack();
	AudioManager::Get()->pushEvent(AudioEvent(AudioEvent::Play, mWeapon->missSoundLabel(), mWeapon));
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
	mWeapon->fastUpdate(dt);

	mStateMachine.getActiveState().fastUpdate(dt);
 }


void Player::slowUpdate(float dt)
{
	Actor::slowUpdate(dt);

	mWeapon->slowUpdate(dt);

	AudioManager* audio = AudioManager::Get();
	if (mWeapon->getCollider()->didHit() && !audio->isPlaying(mWeapon->hitSoundLabel(), mWeapon))
	{
		audio->pushEvent(AudioEvent(AudioEvent::Stop, mWeapon->missSoundLabel(), mWeapon));
		audio->pushEvent(AudioEvent(AudioEvent::Play, mWeapon->hitSoundLabel(), mWeapon));
	}

	updateMapInfo();

	Health* health = static_cast<Health*>(getAttribute(AttributeType::Health));
	if (!health->isDead())
	{
		setMovementAnimation();
		updateMovementSound(audio);
	}

	handleHealthChanges(health);

	if(health->canTakeDamage() && mCollider.gotHit())
	{
		handleHit(audio);
	}
}


void Player::clear()
{
	mWeapon = nullptr;
	mControlOverride = false;
	mHP = nullptr;
	Actor::clear();

	if (mStateMachine.size() > 1)
		mStateMachine.forcePop();
}


Weapon* Player::weapon()
{
	return mWeapon;
}


void Player::render()
{
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
			mAnimator.render(renderRect, mPhysics.flip(), RenderColour(RenderColour::TakeDamage));
		}
		else if (!mHP->canTakeDamage())
		{
			mAnimator.render(renderRect, mPhysics.flip(), RenderColour(RenderColour::Invunerable));
		}
		else
		{
			mColourModTimer.stop();
			mAnimator.render(renderRect, mPhysics.flip());
		}

		mEffects.render();
	}
#if DRAW_PLAYER_RECT
	debugDrawRectOutline(rect(), RenderColour(RenderColour::Green), true);
	debugDrawRectOutline(collider()->scaledRect(), RenderColour(RenderColour::Blue), true);
#endif
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

void Player::meleeAttack()
{
	//mStateMachine.addState(new PlayerAttackState(this));
}



Collider* Player::attackingCollider()
{
	return mWeapon->getCollider();
}


void Player::resetColliders()
{
	mCollider.reset();

	if(mWeapon->getCollider())
		mWeapon->getCollider()->reset();
}


void Player::updateMapInfo()
{
	const Map* map = currentMap();
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
	if (mHP->canTakeDamage())
	{
		Animation::Action action = mPhysics.isMoving() ? Animation::Run : Animation::Idle;
		mAnimator.selectAnimation(action);
	}
}


void Player::updateMovementSound(AudioManager* audio)
{
	Animation::Action currentAction = mAnimator.currentAction();
	if (currentAction == Animation::Run)
	{
		if (mStepTimer.getMilliseconds() > 400)
		{
			audio->pushEvent(AudioEvent(AudioEvent::Play, "PlayerWalk2", this));
			mStepTimer.restart();
		}
	}
	else if (currentAction == Animation::Idle)
	{
		if (audio->isPlaying("PlayerWalk2", this))
			audio->pushEvent(AudioEvent(AudioEvent::Stop, "PlayerWalk2", this));
	}
}


void Player::handleHit(AudioManager* audio)
{
	audio->pushEvent(AudioEvent(AudioEvent::Play, "PlayerHurt", this));

	TraumaEvent* trauma = new TraumaEvent(60);
	mEvents.push(EventPacket(trauma));

	PropertyMap map;
	map[PropertyType::Time] = mAnimator.animationTime(Animation::Hurt);
	Effect* effect = mEffects.pool()->getObject(EffectType::Invunerability);
	effect->fill(map);
	addEffect(effect);

	mColourModTimer.restart();

	mAnimator.selectAnimation(Animation::Hurt);
}


void Player::addEffect(Effect* effect)
{
	Health* health = static_cast<Health*>(getAttribute(AttributeType::Health));
	if (health->canTakeDamage())
	{
		Actor::addEffect(effect);
	}
	else
	{
		mEffects.returnEffect(effect);
	}
}


void Player::handleHealthChanges(Health* health)
{
	if (health->hasChanged())
	{
#if INVUNERABLE
		health->setFullHp();
#endif

		SetUISlider* eventPtr = new SetUISlider("HealthSlider", health->getPercentage());
		EventPacket event(eventPtr);
		mEvents.push(event);

		if (health->isDead() && !mControlOverride)
		{
			mStateMachine.addState(new PlayerDeadState(this));
			AudioManager::Get()->pushEvent(AudioEvent(AudioEvent::Stop, "PlayerWalk2", this));
		}

		health->changedHandled();
	}
}
