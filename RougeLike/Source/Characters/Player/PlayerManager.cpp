#include "pch.h"
#include "PlayerManager.h"

#include "Game/GameData.h"
#include "Player.h"
#include "Map/Map.h"
#include "Characters/Enemies/EnemyManager.h"


PlayerManager::PlayerManager(GameData* gameData) : mGameData(gameData)
{
	player = new Player(gameData);
}


void PlayerManager::init(EnemyManager* enemyManager)
{
	mEnemyManager = enemyManager;
	collisionTracker.addCollider(&player->getCollider());
	player->init(); 
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
}


void PlayerManager::fastUpdate(float dt) 
{
	// resolve collisions before any movement takes place!
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

	VectorF direction = player->getMovement().getDirection();

	if (restrictLeft || restrictRight)
		direction.x = 0.0f;

	if (restrictUp || restrictDown)
		direction.y = 0.0f;

	player->getMovement().setDirection(direction);
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

	if (leftTile.hasCollisionType(MapTile::Right ^ MapTile::Wall))
	{
		float xFuturePosition = point.x + (player->getMovement().getMovementDistance().x * dt);
		return xFuturePosition < leftTile.rect().RightPoint();
	}

	return false;
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

	if (rightTile.hasCollisionType(MapTile::Left ^ MapTile::Wall))
	{
		float xFuturePosition = point.x + (player->getMovement().getMovementDistance().x * dt);
		return xFuturePosition > rightTile.rect().LeftPoint();
	}

	return false;
}


bool PlayerManager::doesCollideTop(VectorF point, float dt)
{
	MapTile currentTile = mGameData->map->getTile(point);
	ASSERT(Warning, currentTile.collisionType() == MapTile::Floor,
		"Player is not on a floor tile, tile at index %d,%d has a %d tile type",
		currentTile.index.x, currentTile.index.y, currentTile.collisionType());

	MapTile upTile = mGameData->map->offsetTile(currentTile, 0, -1);

	if (upTile.hasCollisionType(MapTile::Bot))
	{
		float yFuturePosition = point.y + (player->getMovement().getMovementDistance().y * dt);
		return yFuturePosition < upTile.rect().BotPoint();
	}

	return false;
}


bool PlayerManager::doesCollideBot(VectorF point, float dt)
{
	MapTile currentTile = mGameData->map->getTile(point);
	ASSERT(Warning, currentTile.collisionType() == MapTile::Floor,
		"Player is not on a floor tile, tile at index %d,%d has a %d tile type",
		currentTile.index.x, currentTile.index.y, currentTile.collisionType());

	MapTile downTile = mGameData->map->offsetTile(currentTile, 0, +1);

	if (downTile.hasCollisionType(MapTile::Top))
	{
		float yFuturePosition = point.y + (player->getMovement().getMovementDistance().y * dt);
		return yFuturePosition > downTile.rect().TopPoint();
	}

	return false;
}

