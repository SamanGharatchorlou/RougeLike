#include "pch.h"
#include "ActorManager.h"

#include "Game/GameData.h"
#include "Input/InputManager.h"
#include "UI/UIManager.h"
#include "Audio/AudioManager.h"

#include "Player/Player.h"
#include "Weapons/Melee/MeleeWeapon.h"
#include "Objects/Attributes/StatManager.h"

#include "Game/Camera.h"
#include "Map/Environment.h"

#include "Enemies/Enemy.h"
#include "Enemies/EnemyManager.h"

#include "Collisions/CollisionManager.h"

#include "Graphics/RenderManager.h"

#include "Player/PlayerManager.h"


ActorManager::ActorManager(GameData* gameData) : mGameData(gameData), mPlayer(gameData)
{
	mEnemies = new EnemyManager(gameData);
}


ActorManager::~ActorManager()
{
	delete mEnemies;
}

void ActorManager::load()
{
	mEnemies->load();
}

void ActorManager::init(Environment* environment)
{
	mPlayer.init(environment);
}


void ActorManager::handleInput()
{
	mPlayer.handleInput(mGameData->inputManager);
}

void ActorManager::fastUpdate(float dt)
{
	mPlayer.fastUpdate(dt);
	mEnemies->fastUpdate(dt);
}


void ActorManager::slowUpdate(float dt)
{
	mPlayer.slowUpdate(dt);
	while (mPlayer.events().hasEvent())
		sendEvent(mPlayer.events().pop());
	   
	mEnemies->slowUpdate(dt);
	while (mEnemies->hasEvent())
		sendEvent(mEnemies->popEvent());
}


void ActorManager::render()
{
	mPlayer.render();
	mEnemies->render();
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
		//mPlayer->statManager().gainExp(eventData.mExp);
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
	case Event::Render:
	{
		RenderEvent eventData = static_cast<RenderEvent&>(data);

		RenderPack renderPacket(eventData.mTexture, eventData.mRect, static_cast<RenderLayer>(eventData.mRenderLayer));
		mGameData->renderManager->AddRenderPacket(renderPacket);
		break;
	}
	default:
		break;
	}

}


// player stuff

void ActorManager::exit()
{
	mEnemies->clear();
	mPlayer.exit();
}

/// --- Private Functions --- ///

void ActorManager::initEnemies()
{
	mEnemies->addEnemiesToPool(EnemyType::Devil, 50);
	mEnemies->spawnLevel();
}


void ActorManager::sendEvent(EventPacket eventPacket)
{
	notify(*eventPacket.data);
	eventPacket.free();
}