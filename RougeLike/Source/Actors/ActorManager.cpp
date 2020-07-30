#include "pch.h"
#include "ActorManager.h"

#include "Game/GameData.h"
#include "Graphics/RenderManager.h"
#include "UI/UIManager.h"
#include "Audio/AudioManager.h"

#include "Game/Camera.h"
#include "Map/Environment.h"

#include "Player/PlayerManager.h"
#include "Player/Player.h"
#include "Weapons/Melee/MeleeWeapon.h"
#include "Objects/Attributes/StatManager.h"

#include "Enemies/Enemy.h"
#include "Enemies/EnemyManager.h"


ActorManager::ActorManager(GameData* gameData) : 
	mRendering(gameData->renderManager), mTextures(gameData->textureManager), mPlayer(gameData), mEnemies(gameData) { }



void ActorManager::load(const XMLParser& parser, const Map* map)
{
	mEnemies.load();
}

void ActorManager::init(Environment* environment)
{
	mPlayer.init(environment);
	mEnemies.init(environment);
}


void ActorManager::handleInput(const InputManager* input)
{
	mPlayer.handleInput(input);
}

void ActorManager::fastUpdate(float dt)
{
	mPlayer.fastUpdate(dt);
	mEnemies.fastUpdate(dt);
}


void ActorManager::slowUpdate(float dt)
{
	mPlayer.slowUpdate(dt);
	while (mPlayer.events().hasEvent())
		sendEvent(mPlayer.events().pop());
	   
	mEnemies.slowUpdate(dt);
	while (mEnemies.events().hasEvent())
		sendEvent(mEnemies.events().pop());
}


void ActorManager::render()
{
	mPlayer.render();
	mEnemies.render();
}


void ActorManager::spawnEnemies(const XMLParser& parser, const Map* map)
{
	mEnemies.spawn(parser, map);
}


std::vector<Actor*> ActorManager::getAllEnemies()
{
	std::vector<Actor*> actorList;

	std::vector<Enemy*> enemyList = mEnemies.getActiveEnemies();
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
	//case Event::EnemyDead:
	//{	
	//	EnemyDeadEvent eventData = static_cast<EnemyDeadEvent&>(data);
	//	//mPlayer->statManager().gainExp(eventData.mExp);
	//	mCollisions->removeDefender(CollisionManager::PlayerWeapon_Hit_Enemy, eventData.mEnemy->collider());
	//	break;
	//}
	case Event::UpdateAIPathMap:
	{
		mEnemies.requestEnemyPathUpdates();
		break;
	}
	case Event::UpdateAICostMap:
	{
		mEnemies.updateAIPathCostMap();
		break;
	}
	case Event::Render:
	{
		RenderEvent eventData = static_cast<RenderEvent&>(data);

		RenderPack renderPacket(eventData.mTexture, eventData.mRect, static_cast<RenderLayer>(eventData.mRenderLayer));
		mRendering->AddRenderPacket(renderPacket);
		break;
	}
	default:
		break;
	}

}


// player stuff

void ActorManager::exit()
{
	mEnemies.clear();
	mPlayer.exit();
}

/// --- Private Functions --- ///

void ActorManager::sendEvent(EventPacket eventPacket)
{
	notify(*eventPacket.data);
	eventPacket.free();
}