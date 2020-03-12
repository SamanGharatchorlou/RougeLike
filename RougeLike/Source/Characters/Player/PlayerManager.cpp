
#include "pch.h"
#include "PlayerManager.h"

#include "Game/GameData.h"
#include "Input/InputManager.h"

#include "Player.h"

#include "Weapons/Melee/MeleeWeapon.h"
#include "Weapons/Ranged/RangedWeapon.h"

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

	// TEMP
	//collisionTracker.addCollider(&player->getCollider());
	collisionTracker.addDefender(&player->getCollider());
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


RectF* PlayerManager::getRectRef() { return &player->getRect(); }


std::vector<Collider*> PlayerManager::getWeaponColliders() 
{ 
	return player->getWeapon()->getColliders();
}


void PlayerManager::preProcess() { player->processStateChanges(); }

void PlayerManager::handleInput() 
{ 
	player->handleInput();

	if (mGameData->inputManager->getButton(Button::E).isPressed())
	{
		printf("E pressed\n");

		TraumaEvent event(100);

		notify(Event::Trauma, event);
	}
}


void PlayerManager::fastUpdate(float dt)
{
	// resolve collisions before any movement takes place!
	player->getPhysics().resetAllowedMovement();
	player->getCollider().reset();

	resolveWallCollisions(dt);

	player->fastUpdate(dt);
}


void PlayerManager::slowUpdate(float dt) 
{ 
	player->slowUpdate(dt);

	// TODO: only run this if it needs to, overoptimising?
	updateTrackedColliders();

	// TEMP
	//collisionTracker.checkBaseCollisions();
	collisionTracker.checkCollisions();

	// implement collisions, player getting hit by the enemy
	if (player->getCollider().gotHit())
	{
		Damage damage = player->getCollider().getOtherColliderDamage();
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
	// TEMP
	// Enemy attacking player
	//collisionTracker.clearSubscriptions();
	//collisionTracker.subscribe(mGameData->enemies->getAttackingEnemyColliders());

	collisionTracker.clearAttackers();
	collisionTracker.addAttackers(mGameData->enemies->getAttackingEnemyColliders());
}


void PlayerManager::resolveWallCollisions(float dt)
{
	RectF collisionRect = player->getCollider().getRectBase();

	bool restrictLeft = doesCollideLeft(collisionRect.TopLeft(), dt) || doesCollideLeft(collisionRect.BotLeft(), dt);
	bool restrictRight = doesCollideRight(collisionRect.TopRight(), dt) || doesCollideRight(collisionRect.BotRight(), dt);

	bool restrictUp = doesCollideTop(collisionRect.TopLeft(), dt) || doesCollideTop(collisionRect.TopRight(), dt);
	bool restrictDown = doesCollideBot(collisionRect.BotLeft(), dt) || doesCollideBot(collisionRect.BotRight(), dt);

	player->getPhysics().restrictMovement(Physics::Up, restrictUp);
	player->getPhysics().restrictMovement(Physics::Down, restrictDown);
	player->getPhysics().restrictMovement(Physics::Right, restrictRight);
	player->getPhysics().restrictMovement(Physics::Left, restrictLeft);
}


bool PlayerManager::doesCollideLeft(const VectorF point, float dt) const
{
	bool willCollide = false;
	const MapTile* currentTile = mGameData->map->getTile(point);

	if (currentTile)
	{
		ASSERT(Warning, currentTile->collisionType() == MapTile::Floor,
			"Player is not on a floor tile, tile at index %d,%d has a %d tile type",
			currentTile->index.x, currentTile->index.y, currentTile->collisionType());

		const MapTile* leftTile = mGameData->map->offsetTile(currentTile, -1, 0);

		if (leftTile && leftTile->hasCollisionType(MapTile::Right ^ MapTile::Wall))
		{
			float xFuturePosition = point.x - player->getPhysics().maxMovementDistance(dt);
			willCollide = xFuturePosition < leftTile->rect().RightPoint();
		}
	}
	
	return willCollide;
}


bool PlayerManager::doesCollideRight(const VectorF point, float dt) const
{
	bool willCollide = false;
	const MapTile* currentTile = mGameData->map->getTile(point);

	if (currentTile)
	{
		ASSERT(Warning, currentTile->collisionType() == MapTile::Floor,
			"Player is not on a floor tile, tile at index %d,%d has a %d tile type",
			currentTile->index.x, currentTile->index.y, currentTile->collisionType());

		const MapTile* rightTile = mGameData->map->offsetTile(currentTile, +1, 0);

		if (rightTile && rightTile->hasCollisionType(MapTile::Left ^ MapTile::Wall))
		{
			float xFuturePosition = point.x + player->getPhysics().maxMovementDistance(dt);
			willCollide = xFuturePosition > rightTile->rect().LeftPoint();
		}
	}

	return willCollide;
}


bool PlayerManager::doesCollideTop(const VectorF point, float dt) const
{
	bool willCollide = false;
	const MapTile* currentTile = mGameData->map->getTile(point);

	if (currentTile)
	{
		ASSERT(Warning, currentTile->collisionType() == MapTile::Floor,
			"Player is not on a floor tile, tile at index %d,%d has a %d tile type",
			currentTile->index.x, currentTile->index.y, currentTile->collisionType());

		const MapTile* upTile = mGameData->map->offsetTile(currentTile, 0, -1);

		if (upTile && upTile->hasCollisionType(MapTile::Bot))
		{
			float yFuturePosition = point.y - (player->getPhysics().maxMovementDistance(dt));
			willCollide = yFuturePosition < upTile->rect().BotPoint();
		}
	}

	return willCollide;
}


bool PlayerManager::doesCollideBot(const VectorF point, float dt) const
{
	bool willCollide = false;
	const MapTile* currentTile = mGameData->map->getTile(point);

	if (currentTile)
	{
		ASSERT(Warning, currentTile->collisionType() == MapTile::Floor,
			"Player is not on a floor tile, tile at index %d,%d has a %d tile type",
			currentTile->index.x, currentTile->index.y, currentTile->collisionType());

		const MapTile* downTile = mGameData->map->offsetTile(currentTile, 0, +1);

		if (downTile && downTile->hasCollisionType(MapTile::Top))
		{
			float yFuturePosition = point.y + (player->getPhysics().maxMovementDistance(dt));
			willCollide = yFuturePosition > downTile->rect().TopPoint();
		}
	}

	return willCollide;
}

