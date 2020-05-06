
#include "pch.h"
#include "PlayerManager.h"

#include "Game/GameData.h"
#include "Input/InputManager.h"
#include "Collisions/CollisionManager.h"

#include "Collisions/DamageCollider.h"

#include "Player.h"
#include "Objects/Enemies/EnemyManager.h"

#include "Map/Environment.h"
#include "Map/Map.h"

#include "Weapons/Melee/MeleeWeapon.h"
#include "Objects/Attributes/Level.h"

#include "Objects/Effects/KnockbackEffect.h"

PlayerManager::PlayerManager(GameData* gameData) : mGameData(gameData)
{
	player = new Player(gameData);

	statManager.init(player->propertyBag());
}

PlayerManager::~PlayerManager()
{
	delete player;
	player = nullptr;
}

void PlayerManager::reset()
{
	tileIndex.zero();
	player->reset();
}

void PlayerManager::loadWeaponStash()
{
	weaponStash.load(mGameData->textureManager);
}

void PlayerManager::initCollisions()
{
	std::vector<Collider*> collider{ &player->collider() };

	mGameData->collisionManager->addDefenders(CollisionManager::Enemy_Hit_Player, collider);
	mGameData->collisionManager->addAttackers(CollisionManager::Player_Hit_Collectable, collider);
}


void PlayerManager::selectCharacter(const std::string& character)
{
	player->init(character);
	selectWeapon(player->propertyBag()->pWeapon.get());

	mGameData->collisionManager->removeAllAttackers(CollisionManager::PlayerWeapon_Hit_Enemy);

	updateUIStats();
}


void PlayerManager::selectWeapon(const std::string& weaponName)
{
	Weapon* weapon = weaponStash.getWeapon(weaponName);
	player->equiptWeapon(weapon);
}


RectF* PlayerManager::rect() { return &player->rectRef(); }

void PlayerManager::handleInput() 
{ 
	player->handleInput();

	//// TEMP
	//if (mGameData->inputManager->getButton(Button::E).isPressed())
	//{
	//	TraumaEvent event(100);
	//	notify(Event::Trauma, event);
	//}
}


void PlayerManager::fastUpdate(float dt)
{
	// resolve collisions before any movement takes place!
	player->physics().resetAllowedMovement();

	// Restricts movemoent from input, movement should happen after this
	resolveWallCollisions(mGameData->environment->map(rect()->Center()), dt);

	// Movement, animations, weapon updates
	player->fastUpdate(dt);

	mEffectHandler.fastUpdate(dt);
}


void PlayerManager::slowUpdate(float dt) 
{ 
	mGameData->collisionManager->removeAllAttackers(CollisionManager::PlayerWeapon_Hit_Enemy);

	if (player->weapon()->isAttacking())
	{
		mGameData->collisionManager->addAttackers(CollisionManager::PlayerWeapon_Hit_Enemy, player->weapon()->getColliders());
	}

	// Resolve player getting hit
	if (player->collider().gotHit())
	{
		processHit();
	}

	player->slowUpdate(dt);

	// Level up
	if (player->propertyBag()->pLevel.get().didLevelUp())
	{
		player->updateWeaponStats(player->propertyBag());
		updateUIStats();
	}

	// Update enemy paths when player changes tile
	Vector2D<int> currentTile = mGameData->environment->map(rect()->Center())->index(rect()->Center());
	if (tileIndex != currentTile)
	{
		tileIndex = currentTile;

		UpdateAIPathMapEvent updateAIPathMapEvent;
		notify(Event::UpdateAIPathMap, updateAIPathMapEvent);
	}

	mEffectHandler.slowUpdate(dt);
}


void PlayerManager::updateUIStats()
{
	//UpdateTextBoxEvent attackStat("Atk val", player->propertyBag()->pAttackDmg.get().value());
	//notify(Event::UpdateTextBox, attackStat);

	//UpdateTextBoxEvent defenceStat("Def val", player->propertyBag()->pDefence.get());
	//notify(Event::UpdateTextBox, defenceStat);
}


VectorF PlayerManager::position() const
{
	return player->rect().Center();
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
	player->render();
	mEffectHandler.render();
}


// --- Private Functions --- //

void PlayerManager::processHit()
{
	// Take damage
	const DamageCollider* damageCollider = static_cast<const DamageCollider*>(player->collider().getOtherCollider());
	Health& hp = player->propertyBag()->pHealth.get();
	hp.takeDamage(damageCollider->damage());

	SetHealthBarEvent event(hp);
	notify(Event::SetHealth, event);

	// Knockback
	VectorF source = damageCollider->rect().Center();
	float force = damageCollider->knockbackforce();
	KnockbackEffect* effect = new KnockbackEffect(&player->physics(), source, force);

	mEffectHandler.addEffect(effect);
}


void PlayerManager::resolveWallCollisions(const Map* map, float dt)
{
	RectF collisionRect = player->collider().scaledRect();

	bool restrictLeft =		doesCollideLeft	(map, collisionRect.TopLeft(), dt)	|| 
							doesCollideLeft	(map, collisionRect.BotLeft(), dt);

	bool restrictRight =	doesCollideRight(map, collisionRect.TopRight(), dt) || 
							doesCollideRight(map, collisionRect.BotRight(), dt);

	bool restrictUp =		doesCollideTop	(map, collisionRect.TopLeft(),  dt)	|| 
							doesCollideTop	(map, collisionRect.TopRight(), dt);

	bool restrictDown =		doesCollideBot	(map, collisionRect.BotLeft(),  dt)	|| 
							doesCollideBot	(map, collisionRect.BotRight(), dt);

	player->physics().restrictMovement(Physics::Up, restrictUp);
	player->physics().restrictMovement(Physics::Down, restrictDown);
	player->physics().restrictMovement(Physics::Right, restrictRight);
	player->physics().restrictMovement(Physics::Left, restrictLeft);
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
			float xFuturePosition = point.x - player->physics().maxMovementDistance(dt);
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
			float xFuturePosition = point.x + player->physics().maxMovementDistance(dt);
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
			float yFuturePosition = point.y - (player->physics().maxMovementDistance(dt));
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
			float yFuturePosition = point.y + (player->physics().maxMovementDistance(dt));
			willCollide = yFuturePosition > downTile->rect().TopPoint();
		}
	}

	return willCollide;
}

