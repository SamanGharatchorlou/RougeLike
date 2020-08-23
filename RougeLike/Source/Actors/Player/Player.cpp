#include "pch.h"
#include "Player.h"

#include "Game/Camera/Camera.h"
#include "Audio/AudioManager.h"

#include "Map/Map.h"
#include "Weapons/Weapon.h"
#include "Weapons/Melee/MeleeWeapon.h"

#if DRAW_PLAYER_RECTS
#include "Debug/DebugDraw.h"
#endif



Player::Player() :
	mWeapon(nullptr),
	mControlOverride(false)
{ }


void Player::setCharacter(const XMLNode playerNode)
{
	Actor::setCharacter(playerNode);
}


void Player::handleInput(const InputManager* input)
{
	mPhysics.handleInput(input);
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
	Actor::fastUpdate(dt);

	mWeapon->setPosition(rect().Center());
	mWeapon->fastUpdate(dt);


	VectorF wepRect = mWeapon->rect().Center();
 }


void Player::slowUpdate(float dt)
{
	Actor::slowUpdate(dt);

	mWeapon->slowUpdate(dt);

	Action action = mPhysics.isMoving() ? Action::Run : Action::Idle;
	mAnimator.selectAnimation(action);

	if (collider()->gotHit())
		processHit();
}


void Player::clear()
{
	mTileIndex.zero();
	mWeapon = nullptr;
	mControlOverride = false;
	mBodyCollisions = false;
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
#if DRAW_PLAYER_RECTS
	debugDrawRect(rect(), RenderColour(RenderColour::Green));
	debugDrawRect(mCollider.scaledRect(), RenderColour(RenderColour::Blue));
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

	updateUI();
}


void Player::updateUI()
{
	Health* hp = static_cast<Health*>(getAttribute(AttributeType::Health));
	SetUISlider* healthBar = new SetUISlider("HealthSlider", hp->getPercentage());
	mEvents.push(EventPacket(healthBar));

	//Armor* armor = static_cast<Armor*>(getAttribute(AttributeType::Armor));
	//SetUIBarEvent* armorBar = new SetUIBarEvent("ArmorBar", "BlackArmorBar", armor->getPercentage());
	//mEvents.push(EventPacket(armorBar));
}


bool Player::attemptAttack()
{
	bool canAttack = !mWeapon->isAttacking();

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


void Player::updateMapInfo(Map* map)
{
	if (map->isValidPosition(position()))
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


void Player::enableBodyCollisions(bool isEnabled)
{
	mBodyCollisions = isEnabled;
}
