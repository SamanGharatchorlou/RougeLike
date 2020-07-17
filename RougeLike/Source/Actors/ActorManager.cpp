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


ActorManager::ActorManager(GameData* gameData) : mGameData(gameData), mPlayerCollisions()
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
	if (!mPlayer->userHasControl())
	{
		mPlayer->handleInput(mGameData->inputManager);

		if (mGameData->inputManager->isCursorPressed(Cursor::Left))
		{
			bool didAttack = mPlayer->attemptAttack();
			if (didAttack)
			{
				mPlayerCollisions.clearExcludedColliders(CollisionManager::PlayerWeapon_Hit_Enemy);
				mGameData->audioManager->playSound(mPlayer->weapon()->missSoundLabel(), this);
			}
		}
	}
}

void ActorManager::fastUpdate(float dt)
{
	mPlayerCollisions.fastUpdate(dt, mGameData->environment->map(mPlayer->position()));

	mPlayer->fastUpdate(dt);
	mPlayer->updateCursorPosition(mGameData->inputManager->cursorPosition());


	mEnemies->fastUpdate(dt);
}


void ActorManager::slowUpdate(float dt)
{
	mPlayerCollisions.slowUpdate(mGameData->environment->map(mPlayer->position()));

	mPlayer->slowUpdate(dt);

	if (mPlayer->weapon()->isAttacking())
	{
		mPlayer->updateWeaponHitSound(mGameData->audioManager);
	}

	Map* playerMap = mGameData->environment->map(mPlayer->position());
	mPlayer->updateCurrentTile(playerMap);

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
void ActorManager::selectWeapon(const std::string& weaponName)
{
	Weapon* weapon = weaponStash.getWeapon(weaponName);
	mPlayer->setWeapon(static_cast<MeleeWeapon*>(weapon));

	mPlayerCollisions.refreshWeaponColliders();
}

void ActorManager::exit()
{
	mEnemies->clear();
	mPlayer->reset();
}

/// --- Private Functions --- ///
void ActorManager::initPlayer()
{
	mPlayerCollisions.init(mPlayer, mGameData->collisionManager);

	weaponStash.load(mGameData->textureManager);

	VectorF playerPosition = VectorF(Camera::Get()->rect().Center().x, mGameData->environment->size().y / 2.0f);
	mPlayer->rectRef().SetLeftCenter(playerPosition);

	// Camera
	Camera::Get()->follow(&mPlayer->rectRef());
}


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