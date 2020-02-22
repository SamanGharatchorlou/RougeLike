#include "pch.h"
#include "PlayerManager.h"

#include "Game/GameData.h"
#include "UI/UIManager.h"

#include "Player.h"
#include "Map/Map.h"
#include "Characters/Enemies/EnemyManager.h"


PlayerManager::PlayerManager(GameData* gameData) : mGameData(gameData)
{
	player = new Player(gameData);

	addObserver(mGameData->uiManager);
}


void PlayerManager::init(EnemyManager* enemyManager)
{
	mEnemyManager = enemyManager;
	collisionTracker.addCollider(&player->getCollider());

	player->init("Soldier.xml");
}


RectF* PlayerManager::getRectRef() { return &player->getRect(); }
std::vector<Collider*> PlayerManager::getWeaponColliders() { return player->getWeapon().getColliders(); }

void PlayerManager::preProcess() { player->processStateChanges(); }
void PlayerManager::handleInput() { player->handleInput(); }


void PlayerManager::slowUpdate(float dt) 
{ 
	player->slowUpdate(dt);

	updateTrackedColliders();

	collisionTracker.checkForCollisions();

	// implement collisions, player getting hit by the enemy
	if (player->getCollider().hasCollided())
	{
		if (gotHit == false)
		{
			Damage damage = player->getCollider().getOtherColliderDamage();
			Health maxHealth = player->propertyBag().pHealth.get();

			player->health = player->health - damage;

			SetHealthBarEvent event( maxHealth, player->propertyBag().pHealth.get());
			notify(Event::SetHealth, event);

			gotHit = true;
		}
	}
}


void PlayerManager::fastUpdate(float dt) 
{
	// resolve collisions before any movement takes place!
	player->getPhysics().resetAllowedMovement();

	resolveWallCollisions(dt);

	player->fastUpdate(dt);
}


// --- Private Functions --- //
void PlayerManager::updateTrackedColliders()
{
	collisionTracker.clearSubscriptions();
	collisionTracker.subscribe(mEnemyManager->getAttackingEnemyColliders());
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


bool PlayerManager::doesCollideLeft(VectorF point, float dt)
{
	MapTile currentTile = mGameData->map->getTile(point);

	ASSERT(Warning, currentTile.collisionType() == MapTile::Floor,
		"Player is not on a floor tile, tile at index %d,%d has a %d tile type",
		currentTile.index.x, currentTile.index.y, currentTile.collisionType());

	// make sure left tile request is in bounds
	if (mGameData->map->getIndex(currentTile).x < 1)
		return false;

	MapTile leftTile = mGameData->map->offsetTile(currentTile, -1, 0);

	bool willCollide = false;

	if (leftTile.hasCollisionType(MapTile::Right ^ MapTile::Wall))
	{
		float xFuturePosition = point.x - player->getPhysics().maxMovementDistance(dt);
		willCollide =  xFuturePosition < leftTile.rect().RightPoint();
	}

	return willCollide;
}


bool PlayerManager::doesCollideRight(VectorF point, float dt)
{
	MapTile currentTile = mGameData->map->getTile(point);

	ASSERT(Warning, currentTile.collisionType() == MapTile::Floor,
		"Player is not on a floor tile, tile at index %d,%d has a %d tile type",
		currentTile.index.x, currentTile.index.y, currentTile.collisionType());

	// make sure right tile request is in bounds
	if (currentTile.rect().LeftPoint() >= mGameData->map->size().x)
		return false;

	MapTile rightTile = mGameData->map->offsetTile(currentTile, +1, 0);

	bool willCollide = false;

	if (rightTile.hasCollisionType(MapTile::Left ^ MapTile::Wall))
	{
		float xFuturePosition = point.x + player->getPhysics().maxMovementDistance(dt);
		willCollide = xFuturePosition > rightTile.rect().LeftPoint();
	}

	return willCollide;
}


bool PlayerManager::doesCollideTop(VectorF point, float dt)
{
	MapTile currentTile = mGameData->map->getTile(point);

	ASSERT(Warning, currentTile.collisionType() == MapTile::Floor,
		"Player is not on a floor tile, tile at index %d,%d has a %d tile type",
		currentTile.index.x, currentTile.index.y, currentTile.collisionType());

	MapTile upTile = mGameData->map->offsetTile(currentTile, 0, -1);

	bool willCollide = false;

	if (upTile.hasCollisionType(MapTile::Bot))
	{
		float yFuturePosition = point.y - (player->getPhysics().maxMovementDistance(dt));
		willCollide = yFuturePosition < upTile.rect().BotPoint();
	}

	return willCollide;
}


bool PlayerManager::doesCollideBot(VectorF point, float dt)
{
	MapTile currentTile = mGameData->map->getTile(point);
	ASSERT(Warning, currentTile.collisionType() == MapTile::Floor,
		"Player is not on a floor tile, tile at index %d,%d has a %d tile type",
		currentTile.index.x, currentTile.index.y, currentTile.collisionType());

	MapTile downTile = mGameData->map->offsetTile(currentTile, 0, +1);

	bool willCollide = false;

	if (downTile.hasCollisionType(MapTile::Top))
	{
		float yFuturePosition = point.y + (player->getPhysics().maxMovementDistance(dt));
		willCollide = yFuturePosition > downTile.rect().TopPoint();
	}

	return willCollide;
}

