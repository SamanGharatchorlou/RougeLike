#include "pch.h"
#include "Player.h"

#include "Game/Camera.h"
#include "Game/Cursor.h"
#include "Audio/AudioManager.h"

#include "Map/Map.h"
#include "Weapons/Melee/MeleeWeapon.h"


#if _DEBUG
#include "Debug/DebugDraw.h"
#endif



Player::Player() :
	mWeapon(nullptr),
	mControlOverride(false)
{ }

void Player::setCharacter(XMLParser& parser, TextureManager* textureManager)
{
	Actor::setCharacter(parser, textureManager);
}


void Player::addAbility(const std::string& name)
{
	//mAbilities.add(name);
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
	// Movement, animations, weapon updates
	Actor::fastUpdate(dt);

	mWeapon->setPosition(rect().Center());
	mWeapon->fastUpdate(dt);
 }


void Player::slowUpdate(float dt)
{
	// Actor
	Actor::slowUpdate(dt);

	// Weapon
	mWeapon->slowUpdate(dt);

	// Abilities
	//mAbilities.slowUpdate(dt);
	//if (mAbilities.hasEvent())
	//	pushEvent(mAbilities.popEvent());

	// Character
	Action action = mPhysics.isMoving() ? Action::Run : Action::Idle;
	mAnimator.selectAnimation(action);

	if (collider()->gotHit())
		processHit();
}


void Player::reset()
{
	tileIndex.zero();
	Actor::reset();
}


void Player::setWeapon(MeleeWeapon* weapon)
{
	mWeapon = weapon;
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

	//mAbilities.render();

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

		if (effectCollider->effectCount() == 0)
			printf("zero effects?\n");

		TraumaEvent* trauma = new TraumaEvent(40);
		mEvents.push(EventPacket(trauma));
	}

	updateUI();
}


void Player::updateUI()
{
	// Update UI
	Health* hp = static_cast<Health*>(getProperty("Health"));
	SetHealthBarEvent* hpPtr = new SetHealthBarEvent(*hp);
	mEvents.push(EventPacket(hpPtr));

	Armor* armor = static_cast<Armor*>(getProperty("Armor"));
	SetArmorBarEvent* armorPtr = new SetArmorBarEvent(*armor);
	mEvents.push(EventPacket(armorPtr));
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


void Player::updateCurrentTile(Map* map)
{
	if (map->isValidPosition(position()))
	{
		Vector2D<int> currentTile = map->index(position());
		if (tileIndex != currentTile)
		{
			tileIndex = currentTile;

			UpdateAIPathMapEvent* eventPtr = new UpdateAIPathMapEvent;
			mEvents.push(EventPacket(eventPtr));
		}
	}
}


void Player::enableBodyCollisions(bool isEnabled)
{
	mBodyCollisions = isEnabled;
}
