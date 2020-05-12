
#include "pch.h"
#include "Player.h"

#include "Game/GameData.h"
#include "Game/Cursor.h"
#include "Input/InputManager.h"
#include "Audio/AudioManager.h"

#include "Collisions/CollisionManager.h"
#include "Collisions/DamageCollider.h"
#include "Weapons/Melee/MeleeWeapon.h"

#include "Map/Environment.h"
#include "Map/Map.h"

#include "PlayerPropertyBag.h"

#include "Objects/Effects/KnockbackEffect.h"
#include "Objects/Abilities/Ability.h"
#include "Objects/Effects/SlowEffect.h"
#include "Objects/Effects/HealEffect.h"



Player::Player(GameData* gameData) :
	Actor(gameData), mStateMachine(new NullState),
	mWeapon(nullptr), mWallCollisions(this) { }


void Player::init(const std::string& characterConfig)
{
	initPropertBag(characterConfig);
	Actor::init(characterConfig);

	// Physics
	VectorF size = mAnimator.getSpriteTile()->getRect().Size() * 1.2f;
	mPhysics.setRect(RectF(VectorF(), size));

	initCollider();

	SlowAbility* slowAbility = new SlowAbility(0.25F);
	HealAbility* healAbility = new HealAbility(50.0f);

	mAbilities.add("Slow", slowAbility);
	mAbilities.add("Heal", healAbility);

	mAbilities.select("Heal");
}


void Player::handleInput()
{
	mPhysics.handleInput(mGameData->inputManager);
}



void Player::fastUpdate(float dt)
{
	// Restricts movemoent from input, movement should happen after this
	mWallCollisions.resolveWallCollisions(mGameData->environment->map(rect().Center()), dt);

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

	std::string animation = mPhysics.isMoving() ? "Run" : "Idle";
	mAnimator.selectAnimation(animation);

	mGameData->collisionManager->removeAllAttackers(CollisionManager::PlayerWeapon_Hit_Enemy);

	if (mGameData->inputManager->isCursorPressed(Cursor::Left))
		attack();

	if (weapon()->isAttacking())
		updateAttackingWeapon();

	if (collider()->gotHit())
		processHit();

	updateCurrentTile();

	if (mGameData->inputManager->isPressed(Button::E))
	{
		printf("pressed E\n");
		mAbilities.activate(this);

		Health* hp = static_cast<Health*>(propertyBag()->get("Health"));
		SetHealthBarEvent event(*hp);
		notify(Event::SetHealth, event);
	}
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

	// Starting weapon
	selectWeapon("weapon_katana");
}


void Player::selectWeapon(const std::string& weaponName)
{
	mWeapon = weaponStash.getWeapon(weaponName);
}


void Player::handleEvent(const Event event, EventData& data)
{
	if (event == Event::EnemyDead)
	{
		EnemyDeadEvent eventData = static_cast<EnemyDeadEvent&>(data);
		statManager.gainExp(eventData.mExp);
	}
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

	Actor::render();

	// Weapon
	mWeapon->render();
#endif
}


/// --- Private Functions --- ///

void Player::initPropertBag(const std::string& config)
{
	PlayerPropertyBag* propertyBag = new PlayerPropertyBag;
	propertyBag->readProperties(config);
	setPropertyBag(propertyBag);

	statManager.init(propertyBag);
}

void Player::initCollider()
{
	// Collider
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
	Health* hp = static_cast<Health*>(propertyBag()->get("Health"));
	hp->takeDamage(damageCollider->damage());

	SetHealthBarEvent event(*hp);
	notify(Event::SetHealth, event);

	// Apply knockback
	mEffects.addEffect(new KnockbackEffect(damageCollider));
}


void Player::attack()
{
	if (!mWeapon->isAttacking())
	{
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
			mGameData->audioManager->playSound(mWeapon->hitSoundLabel(), this);
		}
	}
}


void Player::updateCurrentTile()
{
	// Update enemy paths when player changes tile
	Vector2D<int> currentTile = mGameData->environment->map(rect().Center())->index(rect().Center());
	if (tileIndex != currentTile)
	{
		tileIndex = currentTile;

		UpdateAIPathMapEvent updateAIPathMapEvent;
		notify(Event::UpdateAIPathMap, updateAIPathMapEvent);
	}
}
