#include "pch.h"
#include "ActorManager.h"

#include "Game/Data/GameData.h"
#include "Graphics/RenderManager.h"
#include "UI/UIManager.h"
#include "Audio/AudioManager.h"

#include "Game/Camera/Camera.h"
#include "Game/Environment.h"


void ActorManager::load(const XMLParser& parser)
{
	mEnemies.load();
}

void ActorManager::init(GameData* gameData)
{
	mPlayer.init(gameData->environment, gameData->collisionManager, gameData->uiManager->screen(Screen::Game));
	mEnemies.init(gameData->environment, gameData->collisionManager);
}

void ActorManager::clear()
{
	mPlayer.clear();
	mEnemies.clear();
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
	mEnemies.render();
	mPlayer.render();
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
	case Event::LevelUpdated:
	{
		LevelUpdatedEvent& eventData = static_cast<LevelUpdatedEvent&>(data);

		if (eventData.mStatus == LevelUpdatedEvent::Added)
			mEnemies.addNewAIPathMap();
		else if (eventData.mStatus == LevelUpdatedEvent::Removed)
			mEnemies.popAIPathMap();
		else
			DebugPrint(Warning, "Event data not recognised\n");
	}
	default:
		break;
	}
}


// --- Private Functions --- //

void ActorManager::sendEvent(EventPacket eventPacket)
{
	notify(*eventPacket.data);
	eventPacket.free();
}