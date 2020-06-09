
#include "pch.h"
#include "Player.h"

#include "Game/GameData.h"
#include "Game/Cursor.h"
#include "Game/Camera.h"
#include "Input/InputManager.h"
#include "Audio/AudioManager.h"
#include "UI/UIManager.h"
#include "Graphics/Texture.h"

#include "Collisions/CollisionManager.h"
#include "Collisions/DamageCollider.h"
#include "Weapons/Melee/MeleeWeapon.h"

#include "Map/Environment.h"
#include "Map/Map.h"

#include "PlayerPropertyBag.h"

#include "Objects/Abilities/SlowAbility.h"
#include "Objects/Abilities/HealAbility.h"
#include "Objects/Abilities/SpikeAbility.h"
#include "Objects/Abilities/BilnkAbility.h"
#include "Objects/Abilities/ArmorAbility.h"
#include "Objects/Abilities/SmashAbility.h"

#include "Objects/Effects/KnockbackEffect.h"
#include "Objects/Effects/DamageEffect.h"

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
	XMLParser parser;
	parser.parseXML(FileManager::Get()->findFile(FileManager::Configs_Objects, characterConfig));

	// Properties
	initPropertBag(parser);

	// Collider
	mCollider = new Collider;

	Actor::init(parser);

#if _DEBUG
	mCollider->setName("player");
#endif

	addAbility("Smash");
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

	// hot keys
	if (mGameData->inputManager->isPressed(Button::One))
	{
		mAbilities.setState("Smash", Ability::Selected);
	}

	if (mGameData->inputManager->isReleased(Button::One))
	{
		mAbilities.exitSelection();
	}



	if (mGameData->inputManager->isPressed(Button::Space))
		mAbilities.exitSelection();

	if (mGameData->inputManager->isCursorPressed(Cursor::Left) &&
		!mGameData->uiManager->isUsingUI() && !mAbilities.inSelectionMode())
	{
		attack();
	}
}



void Player::fastUpdate(float dt)
{
	// Restricts movemoent from input, movement should happen after this
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

	// Weapon
	if (mPhysics.flip() == SDL_FLIP_NONE && mPhysics.direction().x < 0)
	{
		//mPhysics.setFlip(SDL_FLIP_HORIZONTAL);
	}
	else if (mPhysics.flip() == SDL_FLIP_HORIZONTAL && mPhysics.direction().x > 0)
	{
		//mPhysics.setFlip(SDL_FLIP_NONE);
	}

	mWeapon->setPosition(rect().Center());
	mWeapon->fastUpdate(dt);
	mWeapon->updateAimDirection(mGameData->inputManager->cursorPosition());
}


void Player::slowUpdate(float dt)
{
	Actor::slowUpdate(dt);
	mWeapon->slowUpdate(dt);
	mAbilities.slowUpdate(dt);

	// Handle ability events
	if (mAbilities.hasEvent())
		pushEvent(mAbilities.popEvent());

	Action action = mPhysics.isMoving() ? Action::Run : Action::Idle;
	mAnimator.selectAnimation(action);

	mGameData->collisionManager->removeAllAttackers(CollisionManager::PlayerWeapon_Hit_Enemy);

	if (weapon()->isAttacking())
		updateAttackingWeapon();

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
	Weapon* wep = weaponStash.getWeapon(weaponName);
	mWeapon =  static_cast<MeleeWeapon*>(wep);

	if (mPhysics.flip() == SDL_FLIP_HORIZONTAL)
	{
		mWeapon->leftFlip();
	}
	else if (mPhysics.flip() == SDL_FLIP_NONE)
	{
		mWeapon->rightFlip();
	}
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


/// --- Private Functions --- ///

void Player::initPropertBag(XMLParser& parser)
{
	PlayerPropertyBag* propertyBag = new PlayerPropertyBag;
	propertyBag->readProperties(parser);
	setPropertyBag(propertyBag);

	mStatManager.init(propertyBag);
}


void Player::processHit()
{
	// Take damage
	const DamageCollider* damageCollider = static_cast<const DamageCollider*>(collider()->getOtherCollider());

	mEffects.addEffect(new DamageEffect(damageCollider->damage()));

	// Update UI
	Health* hp = static_cast<Health*>(getProperty("Health"));
	SetHealthBarEvent* hpPtr = new SetHealthBarEvent(*hp);
	pushEvent(EventPacket(hpPtr));

	Armor* armor = static_cast<Armor*>(getProperty("Armor"));
	SetArmorBarEvent* armorPtr = new SetArmorBarEvent(*armor);
	pushEvent(EventPacket(armorPtr));

	// Apply knockback
	mEffects.addEffect(new KnockbackEffect(damageCollider));

	TraumaEvent* trauma = new TraumaEvent(40);
	pushEvent(EventPacket(trauma));
}


void Player::attack()
{
	if (!mWeapon->isAttacking())
	{
		//printf("playing miss\n");
		mGameData->audioManager->playSound(mWeapon->missSoundLabel(), this);

		//mAbilities.activate("Smash");
	}

	mWeapon->attack();
}


void Player::updateAttackingWeapon()
{
	mGameData->collisionManager->addAttackers(CollisionManager::PlayerWeapon_Hit_Enemy, weapon()->getColliders());

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
