
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

#include "Weapons/Melee/MeleeWeapon.h"

#include "Map/Environment.h"
#include "Map/Map.h"

#if _DEBUG
#include "Debug/DebugDraw.h"
#endif



Player::Player(GameData* gameData) :
	Actor(gameData),
	mCollisionManager(this, mGameData->collisionManager),
	mAbilities(mGameData, this),
	mWeapon(nullptr),
	mControlOverride(false)
{ }


void Player::init(const std::string& characterConfig)
{
	Actor::init(characterConfig);

	mCollisionManager.addColliderToTrackers();
	mCollisionManager.enableBodyCollisions(false);

	mEffects.addAttackingEffect(EffectType::Damage);
	mEffects.addAttackingEffect(EffectType::Displacement);
}


void Player::addAbility(const std::string& name)
{
	mAbilities.add(name);
}


void Player::handleInput()
{
	if (!mControlOverride)
		mPhysics.handleInput(mGameData->inputManager);

	mAbilities.handleInput();

	if (mGameData->inputManager->isCursorPressed(Cursor::Left) &&
		!mGameData->uiManager->isUsingUI() && !mAbilities.inSelectionMode())
	{
		attack();
	}
}


void Player::fastUpdate(float dt)
{
	// Restricts movement from input, movement should happen after this
	if (!mControlOverride)
	{
		mCollisionManager.triggerTraps(mGameData->environment->map(position()));
		mCollisionManager.resolveWallCollisions(mGameData->environment->map(position()), dt);
	}

	// Movement, animations, weapon updates
	Actor::fastUpdate(dt);

	VectorF cursorPosition = mGameData->inputManager->cursorPosition();
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

	mWeapon->setPosition(rect().Center());
	mWeapon->fastUpdate(dt);
	mWeapon->updateAimDirection(mGameData->inputManager->cursorPosition());
 }


void Player::effectLoop()
{
	// Return any unused effects back into the pool
	std::vector<EffectCollider*> colliders = mWeapon->getEffectColliders();

	for (int i = 0; i < colliders.size(); i++)
	{
		while (colliders[i]->hasEffects())
		{
			Effect* effect = colliders[i]->popEffect();
			mGameData->effectPool->returnEffect(effect);
		}
	}

	if (mWeapon->didHit())
	{
		// HACK: V V V V V - Update knockback source
		mEffectProperties.setProperty("TargetPositionX", position().x);
		mEffectProperties.setProperty("TargetPositionY", position().y);
		// HACK: ^ ^ ^ ^ ^

		const std::vector<EffectType> effectTypes = mEffects.attackingEffects();
		
		for (int iCol = 0; iCol < colliders.size(); iCol++)
		{
			// Each collider gets it own set of effects
			for (int iEff = 0; iEff < effectTypes.size(); iEff++)
			{
				Effect* effect = mGameData->effectPool->getEffect(effectTypes[iEff]);
				effect->fillData(&mEffectProperties);
				colliders[iCol]->addEffect(effect);
			}
		}
	}
}


void Player::slowUpdate(float dt)
{
	// Actor
	Actor::slowUpdate(dt);

	mCollisionManager.updateTraps(mGameData->environment->map(position()));

	// Weapon
	mWeapon->slowUpdate(dt);
	mCollisionManager.updateWeaponColliders();
	if (weapon()->isAttacking())
	{
		updateWeaponHitSound();
	}

	// Abilities
	mAbilities.slowUpdate(dt);
	if (mAbilities.hasEvent())
		pushEvent(mAbilities.popEvent());

	// Character
	Action action = mPhysics.isMoving() ? Action::Run : Action::Idle;
	mAnimator.selectAnimation(action);

	if (collider()->gotHit())
		processHit();

	updateCurrentTile();
}


void Player::reset()
{
	tileIndex.zero();
	Actor::reset();
}


void Player::loadWeaponStash()
{
	weaponStash.load(mGameData->textureManager);
}


void Player::selectCharacter(const std::string& character)
{
	init(character);
}


void Player::selectWeapon(const std::string& weaponName)
{
	Weapon* weapon = weaponStash.getWeapon(weaponName);
	mWeapon =  static_cast<MeleeWeapon*>(weapon);

	if (mPhysics.flip() == SDL_FLIP_HORIZONTAL)
	{
		mWeapon->leftFlip();
	}
	else if (mPhysics.flip() == SDL_FLIP_NONE)
	{
		mWeapon->rightFlip();
	}

	mCollisionManager.refreshWeaponColliders();

	const MeleeWeaponData* weaponData = mWeapon->getData();
	setEffectProperty("Damage", weaponData->damage.value());
	setEffectProperty("KnockbackForce", weaponData->knockbackForce);
	setEffectProperty("KnockbackDistance", weaponData->knockbackDistance);
}


Weapon* Player::weapon()
{
	return mWeapon;
}


void Player::render()
{
#if DRAW_PLAYER_RECTS
	debugDrawRect(rect(), RenderColour(RenderColour::Green));
	debugDrawRect(mCollider->scaledRect(), RenderColour(RenderColour::Blue));
	debugDrawRects(mWeapon->getRects(), RenderColour(RenderColour::Yellow));
#endif

	mAbilities.render();

	if (mVisibility)
	{
		mWeapon->render();
		Actor::render();
	}
}


void Player::userHasControl(bool control)
{
	mControlOverride = !control;
	
	if (mWeapon)
		mWeapon->overrideCursorControl(!control);
}


// --- Private Functions --- //

void Player::processHit()
{
	EffectCollider* effectCollider = static_cast<EffectCollider*>(mCollider.getOtherCollider());
	processEffects(effectCollider);

	TraumaEvent* trauma = new TraumaEvent(40);
	pushEvent(EventPacket(trauma));

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


void Player::attack()
{
	if (!mWeapon->isAttacking())
	{
		mCollisionManager.clearExcludedList();
		mCollisionManager.enableWeaponCollisions(true);

		mWeapon->attack();

		mGameData->audioManager->playSound(mWeapon->missSoundLabel(), this);
	}
}


void Player::updateWeaponHitSound()
{
	if (mWeapon->didHit())
	{
		if (mGameData->audioManager->isPlaying(mWeapon->missSoundLabel(), this) && mWeapon->canPlayHitSound())
		{
			mGameData->audioManager->playSound(mWeapon->hitSoundLabel(), this);
		}
	}
}


void Player::updateCurrentTile()
{
	// Update enemy paths when player changes tile
	Map* currentMap = mGameData->environment->map(position());

	if (currentMap->isValidPosition(position()))
	{
		Vector2D<int> currentTile = currentMap->index(position());
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
	mCollisionManager.enableBodyCollisions(isEnabled);
}
