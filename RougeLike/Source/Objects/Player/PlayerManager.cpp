
#include "pch.h"
#include "PlayerManager.h"

#include "Game/GameData.h"
#include "Game/Cursor.h"
#include "Input/InputManager.h"
#include "Audio/AudioManager.h"
#include "Collisions/CollisionManager.h"
#include "Collisions/DamageCollider.h"

#include "Map/Environment.h"
#include "Map/Map.h"

#include "Weapons/Melee/MeleeWeapon.h"
#include "Objects/Effects/KnockbackEffect.h"

#include "PlayerPropertyBag.h"
#include "States/State.h"


PlayerManager::PlayerManager(GameData* gameData) :
	Actor(gameData),
	mStateMachine(new NullState),
	mWeapon(nullptr)
{

}

PlayerManager::~PlayerManager()
{
	delete mPropertyBag;
	delete mCollider;
}


void PlayerManager::init(const std::string& characterConfig)
{
	Actor::init(characterConfig);

	// Property bag
	PlayerPropertyBag* propertyBag = new PlayerPropertyBag;
	propertyBag->readProperties(characterConfig);
	setPropertyBag(propertyBag);

	statManager.init(propertyBag);

	// Physics
	Physics::Data physicsData;
	physicsData.force = getPropertyValue("Force");
	physicsData.maxVelocity = getPropertyValue("MaxVelocity");
	mPhysics.init(physicsData);
	VectorF size = mAnimator.getSpriteTile()->getRect().Size() * 1.2f;
	mPhysics.setRect(RectF(VectorF(), size));

	// Collider
	Collider* collider = new Collider;
	VectorF colliderScale = VectorF(1.0f, 0.25f); // only with walls
	collider->init(&mPhysics.rectRef(), colliderScale);
	setCollider(collider);
#if _DEBUG
	mCollider->setName("player");
#endif

}


void PlayerManager::reset()
{
	tileIndex.zero();

	propertyBag()->resetProperties();
	mStateMachine.clearStates();
}


void PlayerManager::loadWeaponStash()
{
	weaponStash.load(mGameData->textureManager);
}

void PlayerManager::initCollisions()
{
	std::vector<Collider*> playerCollider { collider() };
	mGameData->collisionManager->addDefenders(CollisionManager::Enemy_Hit_Player, playerCollider);
	mGameData->collisionManager->addAttackers(CollisionManager::Player_Hit_Collectable, playerCollider);
}


void PlayerManager::selectCharacter(const std::string& character)
{
	init(character);

	// Starting weapon
	selectWeapon("weapon_katana");
}


void PlayerManager::selectWeapon(const std::string& weaponName)
{
	mWeapon = weaponStash.getWeapon(weaponName);
}


void PlayerManager::handleInput() 
{ 
	mPhysics.handleInput(mGameData->inputManager);

	if (mGameData->inputManager->isCursorPressed(Cursor::Left))
	{
		// Play miss sound
		if (!mWeapon->isAttacking())
		{
			mGameData->audioManager->playSound(mWeapon->missSoundLabel(), this);
		}

		mWeapon->attack();
	}
}


void PlayerManager::fastUpdate(float dt)
{
	// Restricts movemoent from input, movement should happen after this
	resolveWallCollisions(mGameData->environment->map(rect().Center()), dt);

	// Movement, animations, weapon updates
	Actor::fastUpdate(dt);

	// Weapon
	mWeapon->fastUpdate(dt);
	mWeapon->updateAnchor(rect().TopLeft());
	mWeapon->updateAimDirection(mGameData->inputManager->cursorPosition());

	mEffectHandler.fastUpdate(dt);
}


void PlayerManager::slowUpdate(float dt) 
{ 
	Actor::slowUpdate(dt);
	mWeapon->slowUpdate(dt);
	mEffectHandler.slowUpdate(dt);
	
	std::string animation = mPhysics.isMoving() ? "Run" : "Idle";
	mAnimator.selectAnimation(animation);

	mGameData->collisionManager->removeAllAttackers(CollisionManager::PlayerWeapon_Hit_Enemy);

	if (weapon()->isAttacking())
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

	// Resolve player getting hit
	if (collider()->gotHit())
	{
		processHit();
	}


	// Update enemy paths when player changes tile
	Vector2D<int> currentTile = mGameData->environment->map(rect().Center())->index(rect().Center());
	if (tileIndex != currentTile)
	{
		tileIndex = currentTile;

		UpdateAIPathMapEvent updateAIPathMapEvent;
		notify(Event::UpdateAIPathMap, updateAIPathMapEvent);
	}
}


void PlayerManager::handleEvent(const Event event, EventData& data)
{
	if (event == Event::EnemyDead)
	{
		EnemyDeadEvent eventData = static_cast<EnemyDeadEvent&>(data);
		statManager.gainExp(eventData.mExp);
	}
}


void PlayerManager::render()
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

	mEffectHandler.render();
}


// --- Private Functions --- //

RectF PlayerManager::renderRect() const
{
	RectF renderRect = rect();
	VectorF size = renderRect.Size();

	// scale render rect by 1.75
	renderRect.SetSize(size * 1.75f);
	VectorF sizeDiff = renderRect.Size() - size;

	renderRect = renderRect.Translate(sizeDiff.x / 2.0f * -1, sizeDiff.y * -1);

	return renderRect;
}



void PlayerManager::processHit()
{
	// Take damage
	const DamageCollider* damageCollider = static_cast<const DamageCollider*>(collider()->getOtherCollider());
	Health* hp = static_cast<Health*>(propertyBag()->get("Health"));
	hp->takeDamage(damageCollider->damage());

	SetHealthBarEvent event(*hp);
	notify(Event::SetHealth, event);

	// Knockback
	VectorF source = damageCollider->rect().Center();
	float force = damageCollider->knockbackforce();
	KnockbackEffect* effect = new KnockbackEffect(physics(), source, force);

	mEffectHandler.addEffect(effect);
}


// Wall Collisions
void PlayerManager::resolveWallCollisions(const Map* map, float dt)
{
	physics()->resetAllowedMovement();

	RectF collisionRect = collider()->scaledRect();

	bool restrictLeft =		doesCollideLeft	(map, collisionRect.TopLeft(), dt)	|| 
							doesCollideLeft	(map, collisionRect.BotLeft(), dt);

	bool restrictRight =	doesCollideRight(map, collisionRect.TopRight(), dt) || 
							doesCollideRight(map, collisionRect.BotRight(), dt);

	bool restrictUp =		doesCollideTop	(map, collisionRect.TopLeft(),  dt)	|| 
							doesCollideTop	(map, collisionRect.TopRight(), dt);

	bool restrictDown =		doesCollideBot	(map, collisionRect.BotLeft(),  dt)	|| 
							doesCollideBot	(map, collisionRect.BotRight(), dt);

	physics()->restrictMovement(Physics::Up, restrictUp);
	physics()->restrictMovement(Physics::Down, restrictDown);
	physics()->restrictMovement(Physics::Right, restrictRight);
	physics()->restrictMovement(Physics::Left, restrictLeft);
}


bool PlayerManager::doesCollideLeft(const Map* map, const VectorF point, float dt) const
{
	bool willCollide = false;
	const MapTile* currentTile = map->tile(point);

	if (currentTile)
	{
		ASSERT(Warning, currentTile->collisionType() == MapTile::Floor,
			"Player is not on a floor tile, tile at index %d,%d has a %d tile type",
			currentTile->index.x, currentTile->index.y, currentTile->collisionType());

		const MapTile* leftTile = map->offsetTile(currentTile, -1, 0);

		if (leftTile && leftTile->hasCollisionType(MapTile::Right ^ MapTile::Wall))
		{
			float xFuturePosition = point.x - physics()->maxMovementDistance(dt);
			willCollide = xFuturePosition < leftTile->rect().RightPoint();
		}
	}
	
	return willCollide;
}


bool PlayerManager::doesCollideRight(const Map* map, const VectorF point, float dt) const
{
	bool willCollide = false;
	const MapTile* currentTile = map->tile(point);

	if (currentTile)
	{
		ASSERT(Warning, currentTile->collisionType() == MapTile::Floor,
			"Player is not on a floor tile, tile at index %d,%d has a %d tile type",
			currentTile->index.x, currentTile->index.y, currentTile->collisionType());

		const MapTile* rightTile = map->offsetTile(currentTile, +1, 0);

		if (rightTile && rightTile->hasCollisionType(MapTile::Left ^ MapTile::Wall))
		{
			float xFuturePosition = point.x + physics()->maxMovementDistance(dt);
			willCollide = xFuturePosition > rightTile->rect().LeftPoint();
		}
	}

	return willCollide;
}


bool PlayerManager::doesCollideTop(const Map* map, const VectorF point, float dt) const
{
	bool willCollide = false;
	const MapTile* currentTile = map->tile(point);

	if (currentTile)
	{
		ASSERT(Warning, currentTile->collisionType() == MapTile::Floor,
			"Player is not on a floor tile, tile at index %d,%d has a %d tile type",
			currentTile->index.x, currentTile->index.y, currentTile->collisionType());

		const MapTile* upTile = map->offsetTile(currentTile, 0, -1);

		if (upTile && upTile->hasCollisionType(MapTile::Bot))
		{
			float yFuturePosition = point.y - physics()->maxMovementDistance(dt);
			willCollide = yFuturePosition < upTile->rect().BotPoint();
		}
	}

	return willCollide;
}


bool PlayerManager::doesCollideBot(const Map* map, const VectorF point, float dt) const
{
	bool willCollide = false;
	const MapTile* currentTile = map->tile(point);

	if (currentTile)
	{
		ASSERT(Warning, currentTile->collisionType() == MapTile::Floor,
			"Player is not on a floor tile, tile at index %d,%d has a %d tile type",
			currentTile->index.x, currentTile->index.y, currentTile->collisionType());

		const MapTile* downTile = map->offsetTile(currentTile, 0, +1);

		if (downTile && downTile->hasCollisionType(MapTile::Top))
		{
			float yFuturePosition = point.y + (physics()->maxMovementDistance(dt));
			willCollide = yFuturePosition > downTile->rect().TopPoint();
		}
	}

	return willCollide;
}

