#include "pch.h"
#include "PlayerManager.h"

#include "Game/GameData.h"
#include "UI/UIManager.h"

#include "Player.h"
#include "Map/Map.h"
#include "Characters/Enemies/EnemyManager.h"

#include "Characters/Attributes/Level.h"


PlayerManager::PlayerManager(GameData* gameData) : mGameData(gameData)
{
	player = new Player(gameData);

	statManager.init(&player->propertyBag());
}

PlayerManager::~PlayerManager()
{
	delete player;
	player = nullptr;
}


void PlayerManager::init()
{
	weaponStash.load(mGameData->textureManager);

	collisionTracker.addCollider(&player->getCollider());
}


void PlayerManager::selectCharacter(const std::string& character)
{
	player->init(character);

	const WeaponData* weaponData = &weaponStash.getData(player->propertyBag().pWeapon.get());
	player->equiptWeapon(weaponData);
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
			Health& hp = player->propertyBag().pHealth.get();
			hp.takeDamage(damage);

			SetHealthBarEvent event( hp );
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


void PlayerManager::handleEvent(const Event event, EventData& data)
{
	if (event == Event::EnemyDead)
	{
		EnemyDeadEvent eventData = static_cast<EnemyDeadEvent&>(data);

		statManager.gainExp(eventData.mExp);
	}
}


// --- Private Functions --- //
void PlayerManager::updateTrackedColliders()
{
	collisionTracker.clearSubscriptions();
	collisionTracker.subscribe(mGameData->enemies->getAttackingEnemyColliders());
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
	const MapTile* currentTile = mGameData->map->getTile(point);

	ASSERT(Warning, currentTile->collisionType() == MapTile::Floor,
		"Player is not on a floor tile, tile at index %d,%d has a %d tile type",
		currentTile->index.x, currentTile->index.y, currentTile->collisionType());

	bool willCollide = false;

	if (currentTile)
	{
		const MapTile* leftTile = mGameData->map->offsetTile(currentTile, -1, 0);

		if (leftTile->hasCollisionType(MapTile::Right ^ MapTile::Wall))
		{
			float xFuturePosition = point.x - player->getPhysics().maxMovementDistance(dt);
			willCollide = xFuturePosition < leftTile->rect().RightPoint();
		}
	}
	
	return willCollide;
}


bool PlayerManager::doesCollideRight(const VectorF point, float dt) const
{
	const MapTile* currentTile = mGameData->map->getTile(point);

	ASSERT(Warning, currentTile->collisionType() == MapTile::Floor,
		"Player is not on a floor tile, tile at index %d,%d has a %d tile type",
		currentTile->index.x, currentTile->index.y, currentTile->collisionType());

	bool willCollide = false;

	if (currentTile)
	{
		const MapTile* rightTile = mGameData->map->offsetTile(currentTile, +1, 0);

		if (rightTile->hasCollisionType(MapTile::Left ^ MapTile::Wall))
		{
			float xFuturePosition = point.x + player->getPhysics().maxMovementDistance(dt);
			willCollide = xFuturePosition > rightTile->rect().LeftPoint();
		}
	}

	return willCollide;
}


bool PlayerManager::doesCollideTop(const VectorF point, float dt) const
{
	const MapTile* currentTile = mGameData->map->getTile(point);

	ASSERT(Warning, currentTile->collisionType() == MapTile::Floor,
		"Player is not on a floor tile, tile at index %d,%d has a %d tile type",
		currentTile->index.x, currentTile->index.y, currentTile->collisionType());

	bool willCollide = false;

	if (currentTile)
	{
		const MapTile* upTile = mGameData->map->offsetTile(currentTile, 0, -1);

		if (upTile->hasCollisionType(MapTile::Bot))
		{
			float yFuturePosition = point.y - (player->getPhysics().maxMovementDistance(dt));
			willCollide = yFuturePosition < upTile->rect().BotPoint();
		}
	}

	return willCollide;
}


bool PlayerManager::doesCollideBot(const VectorF point, float dt) const
{
	const MapTile* currentTile = mGameData->map->getTile(point);

	ASSERT(Warning, currentTile->collisionType() == MapTile::Floor,
		"Player is not on a floor tile, tile at index %d,%d has a %d tile type",
		currentTile->index.x, currentTile->index.y, currentTile->collisionType());

	bool willCollide = false;

	if (currentTile)
	{
		const MapTile* downTile = mGameData->map->offsetTile(currentTile, 0, +1);

		if (downTile->hasCollisionType(MapTile::Top))
		{
			float yFuturePosition = point.y + (player->getPhysics().maxMovementDistance(dt));
			willCollide = yFuturePosition > downTile->rect().TopPoint();
		}
	}

	return willCollide;
}

