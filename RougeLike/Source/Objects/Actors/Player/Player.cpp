
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
	mStateMachine(new NullState),
	mAbilities(mGameData),
	mWeapon(nullptr), 
	mWallCollisions(this),
	mControlOverride(false)
{ }


void Player::init(const std::string& characterConfig)
{
	Actor::init(characterConfig);

	mEffects.addAttackingEffect(EffectType::Damage);
	mEffects.addAttackingEffect(EffectType::Displacement);
}


void Player::addAbility(const std::string& name)
{
	Ability* ability = mAbilities.createNewAbility(name);
	mAbilities.add(ability);
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
	if(!mControlOverride)
		mWallCollisions.resolveWallCollisions(mGameData->environment->map(position()), dt);

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
		// HACK: V V V V V
		const MeleeWeaponData* weaponData = mWeapon->getData();

		mEffectProperties.setProperty("TargetPositionX", position().x);
		mEffectProperties.setProperty("TargetPositionY", position().y);

		setEffectProperty("Damage", weaponData->damage.value());
		setEffectProperty("KnockbackForce", weaponData->knockbackForce);
		setEffectProperty("KnockbackDistance", weaponData->knockbackDistance);
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

	// Weapon
	mWeapon->slowUpdate(dt);
	updateWeaponCollisions();

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
	mStateMachine.clearStates();
	Actor::reset();
}


void Player::loadWeaponStash()
{
	weaponStash.load(mGameData->textureManager);
}


void Player::initCollisions()
{
	std::vector<Collider*> playerCollider { collider() };
	mGameData->collisionManager->addDefenders(CollisionManager::Enemy_Hit_Player, playerCollider);
	mGameData->collisionManager->addAttackers(CollisionManager::Player_Hit_Collectable, playerCollider);
}


void Player::selectCharacter(const std::string& character)
{
	init(character);
}


void Player::selectWeapon(const std::string& weaponName)
{
	Weapon* weapon = weaponStash.getWeapon(weaponName);
	mWeapon =  static_cast<MeleeWeapon*>(weapon);
	updateProperties();

	if (mPhysics.flip() == SDL_FLIP_HORIZONTAL)
	{
		mWeapon->leftFlip();
	}
	else if (mPhysics.flip() == SDL_FLIP_NONE)
	{
		mWeapon->rightFlip();
	}

	mGameData->collisionManager->removeAllAttackers(CollisionManager::PlayerWeapon_Hit_Enemy);
	mGameData->collisionManager->addAttackers(CollisionManager::PlayerWeapon_Hit_Enemy, mWeapon->getColliders());
}

void Player::updateProperties()
{
	const MeleeWeaponData* data = mWeapon->getData();
	//mPropertyBag->get("Damage")->setValue(data->damage.value());
	//mPropertyBag->get("KnockbackDistance")->setValue(data->knockbackDistance);
	//mPropertyBag->get("KnockbackForce")->setValue(data->knockbackForce);
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
		CollisionTracker* simpleTracker = mGameData->collisionManager->getTracker(CollisionManager::PlayerWeapon_Hit_Enemy);
		ComplexCollisionTracker* complexTracker = static_cast<ComplexCollisionTracker*>(simpleTracker);
		complexTracker->clearExcluddDefenders();

		mWeapon->attack();

		//printf("playing miss\n");
		mGameData->audioManager->playSound(mWeapon->missSoundLabel(), this);
	}
}


void Player::updateAttackingWeapon()
{
	if (mWeapon->didHit())
	{
		// Play hit sound
		if (mGameData->audioManager->isPlaying(mWeapon->missSoundLabel(), this) && mWeapon->canPlayHitSound())
		{
			//printf("playing hit\n");
			mGameData->audioManager->playSound(mWeapon->hitSoundLabel(), this);
		}
	}
}


void Player::updateWeaponCollisions()
{
	CollisionTracker* simpleTracker = mGameData->collisionManager->getTracker(CollisionManager::PlayerWeapon_Hit_Enemy);
	ComplexCollisionTracker* complexTracker = static_cast<ComplexCollisionTracker*>(simpleTracker);

	if (weapon()->isAttacking())
	{
		complexTracker->setCheckingStatus(true);
		updateAttackingWeapon();

		std::vector<Collider*> enemies = complexTracker->defenders();

		for (int i = 0; i < enemies.size(); i++)
		{
			if (enemies[i]->gotHit())
			{
				complexTracker->addExcludedDefender(enemies[i]);
			}
		}
	}
	else
	{
		complexTracker->setCheckingStatus(false);
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