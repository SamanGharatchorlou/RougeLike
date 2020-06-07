#include "pch.h"
#include "ActorManager.h"

#include "Game/GameData.h"

#include "Player/Player.h"
#include "Objects/Attributes/StatManager.h"

#include "Game/Camera.h"
#include "Map/Environment.h"

#include "Enemies/Enemy.h"
#include "Enemies/EnemyManager.h"

#include "Collisions/CollisionManager.h"


ActorManager::ActorManager(GameData* gameData) : mGameData(gameData)
{
	mPlayer = new Player(gameData);
	mEnemies = new EnemyManager(gameData);
}


ActorManager::~ActorManager()
{
	delete mPlayer;
	delete mEnemies;
}


void ActorManager::init()
{
	initPlayer();
	initEnemies();
}

void ActorManager::handleInput()
{
	mPlayer->handleInput();
}

void ActorManager::fastUpdate(float dt)
{
	mPlayer->fastUpdate(dt);
	mEnemies->fastUpdate(dt);
}


void ActorManager::slowUpdate(float dt)
{
	mPlayer->slowUpdate(dt);
	while (mPlayer->hasEvent())
		sendEvent(mPlayer->popEvent());

	mEnemies->slowUpdate(dt);
	while (mEnemies->hasEvent())
		sendEvent(mEnemies->popEvent());
}


void ActorManager::render()
{
	mPlayer->render();
	mEnemies->render();
}

Actor* ActorManager::playerActor()
{
	return mPlayer;
}

std::vector<Actor*> ActorManager::getAllActors()
{
	std::vector<Actor*> actorList;

	std::vector<Enemy*> enemyList = mEnemies->getActiveEnemies();
	actorList.reserve(enemyList.size() + 1);

	// Add player
	actorList.push_back(mPlayer);

	// Add enemies
	for (int i = 0; i < enemyList.size(); i++)
	{
		actorList.push_back(enemyList[i]);
	}

	return actorList;
}


std::vector<Actor*> ActorManager::getAllEnemies()
{
	std::vector<Actor*> actorList;

	std::vector<Enemy*> enemyList = mEnemies->getActiveEnemies();
	actorList.reserve(enemyList.size());

	for (int i = 0; i < enemyList.size(); i++)
	{
		actorList.push_back(enemyList[i]);
	}

	return actorList;
}


void ActorManager::handleEvent(EventData& data)
{
	switch (data.eventType)
	{
	case Event::EnemyDead:
	{
		EnemyDeadEvent eventData = static_cast<EnemyDeadEvent&>(data);
		mPlayer->statManager().gainExp(eventData.mExp);
		mGameData->collisionManager->removeDefender(CollisionManager::PlayerWeapon_Hit_Enemy, eventData.mEnemy->collider());
		break;
	}
	case Event::UpdateAIPathMap:
	{
		mEnemies->requestEnemyPathUpdates();
		break;
	}
	case Event::UpdateAICostMap:
	{
		mEnemies->updateAIPathCostMap();
		break;
	}
	default:
		break;
	}

}



/// --- Private Functions --- ///
void ActorManager::initPlayer()
{
	mPlayer->initCollisions();

	VectorF playerPosition = VectorF(Camera::Get()->getCenter().x, mGameData->environment->size().y / 2.0f);
	mPlayer->rectRef().SetLeftCenter(playerPosition);

	// Camera
	Camera::Get()->follow(&mPlayer->rectRef());
}


void ActorManager::initEnemies()
{
	//mEnemies->addEnemiesToPool(EnemyType::Imp, 50);
	//mEnemies->addEnemiesToPool(EnemyType::Angel, 50);
	mEnemies->addEnemiesToPool(EnemyType::Devil, 50);

	mEnemies->setTarget(&mGameData->actors->player()->rectRef());
	mEnemies->spawnLevel();
}


void ActorManager::sendEvent(EventPacket eventPacket)
{
	notify(*eventPacket.data);
	eventPacket.free();
}