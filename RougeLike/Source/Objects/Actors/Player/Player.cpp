
#include "pch.h"
#include "Player.h"

#include "Game/GameData.h"
#include "Game/Cursor.h"
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

#include "Objects/Effects/KnockbackEffect.h"
#include "Objects/Effects/DamageEffect.h"


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
	initPropertBag(characterConfig);
	Actor::init(characterConfig);

	// Physics
	VectorF size = mAnimator.getSpriteTile()->getRect().Size() * 1.2f;
	mPhysics.setRect(RectF(VectorF(), size));

	initCollider();

	//// TODO: Get these strings from a set so they match up with the UI?
	//addAbility("Slow", new SlowAbility(0.25F));
	//addAbility("Heal", new HealAbility(50.0f));
	//addAbility("Spikes", new SpikeAbility(Damage(100.0f), 300.0f));
	//addAbility("Blink", new BlinkAbility(500.0f));
	////addAbility("Armor", new ArmorAbility(50.0f));
}


void Player::addAbility(const std::string& name, Ability* ability)
{
	mAbilities.add(name, ability);
}

void Player::handleInput()
{
	if (!mControlOverride)
		mPhysics.handleInput(mGameData->inputManager);

	mAbilities.handleInput();

	if (mGameData->inputManager->isPressed(Button::Space))
		mAbilities.exitSelection();

	if (mGameData->inputManager->isCursorPressed(Cursor::Left) && 
		!mGameData->uiManager->isUsingUI() && !mAbilities.inSelectionMode())
		attack();
}



void Player::fastUpdate(float dt)
{
	// Restricts movemoent from input, movement should happen after this
	if(!mControlOverride)
		mWallCollisions.resolveWallCollisions(mGameData->environment->map(position()), dt);

	// Movement, animations, weapon updates
	Actor::fastUpdate(dt);

	// Weapon
	mWeapon->fastUpdate(dt);
	mWeapon->updateAnchor(rect().TopLeft());
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

	std::string animation = mPhysics.isMoving() ? "Run" : "Idle";
	mAnimator.selectAnimation(animation);

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
	mWeapon = weaponStash.getWeapon(weaponName);
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

	mAbilities.render();

	if (mVisibility)
	{
		Actor::render();
		mWeapon->render();
	}
#endif
}


void Player::userHasControl(bool control)
{
	mControlOverride = !control;
	
	if (mWeapon)
		mWeapon->overrideCursorControl(!control);
}


/// --- Private Functions --- ///

void Player::initPropertBag(const std::string& config)
{
	PlayerPropertyBag* propertyBag = new PlayerPropertyBag;
	propertyBag->readProperties(config);
	setPropertyBag(propertyBag);

	mStatManager.init(propertyBag);
}


void Player::initCollider()
{
	Collider* collider = new Collider;
	VectorF colliderScale = VectorF(1.0f, 0.25f); // only with walls
	collider->init(&mPhysics.rectRef(), colliderScale);
	setCollider(collider);
#if _DEBUG
	mCollider->setName("player");
#endif
}


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
		printf("playing miss\n");
		mGameData->audioManager->playSound(mWeapon->missSoundLabel(), this);
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
			printf("playing hit\n");
			mGameData->audioManager->playSound(mWeapon->hitSoundLabel(), this);
		}
	}
}


void Player::updateCurrentTile()
{
	// Update enemy paths when player changes tile
	Map* currentMap = mGameData->environment->map(rect().Center());

	if (currentMap->isValidPosition(rect().Center()))
	{
		Vector2D<int> currentTile = currentMap->index(rect().Center());
		if (tileIndex != currentTile)
		{
			tileIndex = currentTile;

			UpdateAIPathMapEvent* eventPtr = new UpdateAIPathMapEvent;
			pushEvent(EventPacket(eventPtr));
		}
	}
}
