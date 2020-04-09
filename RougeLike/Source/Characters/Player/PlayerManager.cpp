
#include "pch.h"
#include "PlayerManager.h"

#include "Game/GameData.h"
#include "Input/InputManager.h"

#include "Player.h"

#include "Weapons/Melee/MeleeWeapon.h"
#include "Weapons/Ranged/RangedWeapon.h"

#include "Map/MapLevel.h" // TODO: do I need both map level and map here?
#include "Map/Map.h"
#include "Characters/Enemies/EnemyManager.h"

#include "Characters/Attributes/Level.h"


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


void PlayerManager::init()
{
	weaponStash.load(mGameData->textureManager);

	collisionTracker.addDefender(&player->collider());
}


void PlayerManager::selectCharacter(const std::string& character)
{
	player->init(character);
	selectWeapon(player->propertyBag()->pWeapon.get());

	updateUIStats();
}


void PlayerManager::selectWeapon(const std::string& weaponName)
{
	Weapon* weapon = weaponStash.getWeapon(weaponName);
	player->equiptWeapon(weapon);
}


RectF* PlayerManager::getRectRef() { return &player->rectRef(); }


std::vector<Collider*> PlayerManager::getWeaponColliders() 
{ 
	return player->weapon()->getColliders();
}


void PlayerManager::preProcess() { player->processStateChanges(); }

void PlayerManager::handleInput() 
{ 
	player->handleInput();

	// TEMP
	if (mGameData->inputManager->getButton(Button::E).isPressed())
	{
		TraumaEvent event(100);

		notify(Event::Trauma, event);
	}
}


void PlayerManager::fastUpdate(float dt)
{
	// resolve collisions before any movement takes place!
	player->physics().resetAllowedMovement();
	player->collider().reset();

	RectF rect = *getRectRef();
	resolveWallCollisions(mGameData->level->map(rect.Center()), dt);

	player->fastUpdate(dt);
}


void PlayerManager::slowUpdate(float dt) 
{ 
	player->slowUpdate(dt);

	// Test for enemy attacking player collisions
	updateTrackedColliders();
	collisionTracker.checkCollisions();

	// implement collisions, player getting hit by the enemy
	if (player->collider().gotHit())
	{
		Damage damage = player->collider().getOtherColliderDamage();
		Health& hp = player->propertyBag()->pHealth.get();
		hp.takeDamage(damage);

		SetHealthBarEvent event( hp );
		notify(Event::SetHealth, event);
	}

	if (player->propertyBag()->pLevel.get().didLevelUp())
	{
		player->updateWeaponStats(player->propertyBag());

		updateUIStats();
	}
}
 

void PlayerManager::updateUIStats()
{
	UpdateTextBoxEvent attackStat("Atk val", player->propertyBag()->pAttackDmg.get().value());
	notify(Event::UpdateTextBox, attackStat);

	UpdateTextBoxEvent defenceStat("Def val", player->propertyBag()->pDefence.get());
	notify(Event::UpdateTextBox, defenceStat);
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
}


// --- Private Functions --- //
void PlayerManager::updateTrackedColliders()
{
	// Enemy attacking player
	collisionTracker.clearAttackers();
	collisionTracker.addAttackers(mGameData->enemies->getAttackingColliders());
}


void PlayerManager::resolveWallCollisions(const Map* map, float dt)
{
	RectF collisionRect = player->collider().getRectBase();

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
	const MapTile* currentTile = map->getTile(point);

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
	const MapTile* currentTile = map->getTile(point);

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
	const MapTile* currentTile = map->getTile(point);

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
	const MapTile* currentTile = map->getTile(point);

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

