#include "pch.h"
#include "ActorManager.h"

#include "Game/Data/GameData.h"
#include "Graphics/RenderManager.h"
#include "UI/UIManager.h"
#include "Audio/AudioManager.h"

#include "Game/Camera/Camera.h"
#include "Game/Environment.h"


ActorManager::~ActorManager()
{
	clear();
}


void ActorManager::loadPools()
{
	mEnemies.loadPools();
}

void ActorManager::init(GameData* gameData)
{
	mPlayer.init(gameData->environment, gameData->uiManager->screen(ScreenType::Game));
	mEnemies.init(gameData->environment);

	mNetworkPlayer.init(gameData->environment, gameData->uiManager->screen(ScreenType::Game));

	mPlayerActor.push_back(mPlayer.get());
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

	mNetworkPlayer.fastUpdate(dt);
}


void ActorManager::slowUpdate(float dt)
{
	updateActiveEnemyActors();

	mPlayer.slowUpdate(dt);
	while (mPlayer.events().hasEvent())
		sendEvent(mPlayer.events().pop());

	mNetworkPlayer.slowUpdate(dt);
	while (mNetworkPlayer.events().hasEvent())
		sendEvent(mNetworkPlayer.events().pop());
	   
	mEnemies.slowUpdate(dt);
	while (mEnemies.events().hasEvent())
		sendEvent(mEnemies.events().pop());

	// Reset colliders at the end of the frame
#if !TRACK_COLLISIONS // Need to keep the hit data until after rendering
	resetColliders();
#endif
}


void ActorManager::resetColliders()
{
	mPlayer.resetColliders();
	mEnemies.resetColliders();

	mNetworkPlayer.resetColliders();
}


void ActorManager::render()
{
	mEnemies.render();
	mPlayer.render();

	mNetworkPlayer.render();

#if TRACK_COLLISIONS // Need to keep the hit data until after rendering
	resetColliders();
#endif
}


void ActorManager::updateActiveEnemyActors()
{
	mActiveEnemyActors = mEnemies.getActiveEnemyActors();
}


void ActorManager::handleEvent(EventData& data)
{
	switch (data.eventType)
	{
	case Event::LevelUpdated:
	{
		LevelUpdatedEvent& eventData = static_cast<LevelUpdatedEvent&>(data);

		if (eventData.mStatus == LevelUpdatedEvent::Added)
			mEnemies.openNewMapLevel();
		else if (eventData.mStatus == LevelUpdatedEvent::Removed)
			mEnemies.closeLastMapLevel();
		else
			DebugPrint(Warning, "Event data not recognised\n");
		break;
	}
	case Event::EnemyDead:
	{
		EnemyDeadEvent& eventData = static_cast<EnemyDeadEvent&>(data);

		int exp = eventData.mExp;
		mPlayer.addExp(exp);
		break;
	}
	case Event::PlayerDead:
	{
		// Turn enemies 'off'
		std::vector<Enemy*>& enemies = mEnemies.getActiveEnemies();
		for (Enemy* enemy : enemies)
		{
			enemy->setTarget(nullptr);
		}
		break;
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