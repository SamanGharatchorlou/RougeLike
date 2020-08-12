#include "pch.h"
#include "ActorManager.h"

#include "Game/Data/GameData.h"
#include "Graphics/RenderManager.h"
#include "UI/UIManager.h"
#include "Audio/AudioManager.h"

#include "Game/Camera/Camera.h"
#include "Game/Environment.h"


ActorManager::ActorManager(GameData* gameData) : 
	mTextures(gameData->textureManager), mPlayer(gameData), mEnemies(gameData) { }



void ActorManager::load(const XMLParser& parser)
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


void ActorManager::spawnEnemies(const XMLNode spawnNode, const Map* map)
{
	mEnemies.spawn(spawnNode, map);
}


std::vector<Actor*> ActorManager::getAllEnemies()
{
	return mEnemies.getActiveEnemies();
}


void ActorManager::handleEvent(EventData& data)
{
	switch (data.eventType)
	{
		// TODO: profile how frequently this is run, get a number of calls per time
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
	default:
		break;
	}
}


void ActorManager::exit()
{
	mEnemies.clear();
	mPlayer.exit();
}

// --- Private Functions --- //

void ActorManager::sendEvent(EventPacket eventPacket)
{
	notify(*eventPacket.data);
	eventPacket.free();
}