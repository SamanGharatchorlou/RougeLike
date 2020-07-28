#include "pch.h"
#include "Actors/Enemies/EnemyManager.h"

#include "Game/GameData.h"
#include "Collisions/CollisionManager.h"
#include "Map/Environment.h"
#include "Game/Camera.h"
#include "Actors/ActorManager.h"
#include "Actors/Player/Player.h"

#include "Types/Devil.h"

// State specific updates
#include "Actors/Enemies/EnemyStates/EnemyRun.h"
#include "Actors/Enemies/EnemyStates/EnemyAttack.h"

#if DRAW_AI_PATH_COSTMAP
#include "UI/Elements/UITextBox.h"
#endif


EnemyManager::EnemyManager(GameData* gameData) : 
	mCollisions(gameData->collisionManager), pathUpdateRequests(0), pathUpdateStaggerCounter(0)
{
	updateTimer.start();
}


EnemyManager::~EnemyManager()
{
	mActiveEnemies.clear();
	mPool.freeAll();
}


void EnemyManager::clear()
{
	clearAllEnemies();
	mPathMap.clear();
}

void EnemyManager::load()
{
	// Set ai path map
	generateAIPathMap();
	mPool.addNewObjects(EnemyType::Devil, 50);
}

void EnemyManager::init(Environment* environment)
{
	mEnvironment = environment;
}


void EnemyManager::fastUpdate(float dt)
{
	for (int i = 0; i < mActiveEnemies.size(); i++)
	{
		mActiveEnemies[i]->fastUpdate(dt);
	}
}


void EnemyManager::slowUpdate(float dt)
{
	for (std::vector<Enemy*>::iterator iter = mActiveEnemies.begin(); iter != mActiveEnemies.end(); iter++)
	{
		Enemy* enemy = *iter;

		// Update enemies
		enemy->slowUpdate(dt);

		// Handle enemy messages
		while (enemy->events().hasEvent())
			mEvents.push(enemy->events().pop());

		// Clear out dead enemies
		if (enemy->state() == EnemyState::Exit)
		{
			clearAndRemove(iter);

			if (iter == mActiveEnemies.end())
				break;
		}
	}

	if (pathUpdateStaggerCounter != 0 || pathUpdateRequests)
	{
		updateEnemyPaths();
	}

	mCollisions.updateAttackingColliders(attackingColliders());
}




void EnemyManager::generateAIPathMap()
{
	mPathMap.build(mEnvironment->primaryMap(), 4, 4);
}



void EnemyManager::spawn(EnemyType type, EnemyState::Type state, VectorF position, TextureManager* textureManager, EffectPool* effectPool)
{
#if LIMIT_ENEMY_SPAWNS
	if (mActiveEnemies.size() >= LIMIT_ENEMY_SPAWNS - 1)
		return;
#endif
	
	std::string enemyType;
	type >> enemyType;

	XMLParser parser(FileManager::Get()->findFile(FileManager::Configs_Objects, enemyType));

	Enemy* enemy = mPool.getObject(type);

	enemy->setCharacter(parser, textureManager);
	enemy->readEffects(parser, effectPool);

	// TODO: add some kind of reset attribute thing here?
	//enemy->propertyBag().pHealth.get().setFullHp();
	enemy->set(mEnvironment);

	enemy->setMap(&mPathMap);
	enemy->spawn(state, position);
#if !IGNORED_BY_ENEMIES
	enemy->setTarget(mGameData->actors->player());
#endif

	mCollisions.add(enemy->collider());

	mActiveEnemies.push_back(enemy);
}


// If player moves tile or an enemy moves a tile update this bad boi
void EnemyManager::updateEnemyPaths()
{
	mPathMap.costMap().setAllValues(1);
	updateAIPathCostMap();

	int loopCount = 0;

	for (; pathUpdateStaggerCounter < mActiveEnemies.size(); pathUpdateStaggerCounter++)
	{
		// Only allow a number of these (expensive) updates per frame, reduce stuttering
		if (loopCount++ >= 3)
			return;

		if (mActiveEnemies[pathUpdateStaggerCounter]->state() == EnemyState::Run)
		{
			EnemyRun& runState = static_cast<EnemyRun&>(mActiveEnemies[pathUpdateStaggerCounter]->getStateMachine()->getActiveState());

			// No need to update anything if in attack range
			if (!runState.inAttackRange())
			{
				runState.updatePath();
			}
		}
	}

	if (pathUpdateStaggerCounter == mActiveEnemies.size())
	{
		pathUpdateStaggerCounter = 0;
		pathUpdateRequests--;

		ASSERT(Warning, pathUpdateRequests >= 0, "Cannot have negative path update requests, count: %d\n", pathUpdateRequests);

		// No reason to have more than 1 request queued up after completing a request
		pathUpdateRequests = clamp(pathUpdateStaggerCounter, 0, 1);
	}
}


void EnemyManager::updateAIPathCostMap()
{
	Grid<int>& AICostMap = mPathMap.costMap();
	AICostMap.setAllValues(1);

	for (int i = 0; i < mActiveEnemies.size(); i++)
	{
		// Current tile
		Index index = mPathMap.index(mActiveEnemies[i]->position());
		AICostMap[index] += 10;

		// Immediate surrounding tiles
		Index surroundingIndexsLayer1[8]{
			Index(index + Index(-1,-1)),
			Index(index + Index(+0, -1)),
			Index(index + Index(+1,-1)),

			Index(index + Index(-1,0)),
			Index(index + Index(+1,0)),

			Index(index + Index(-1,+1)),
			Index(index + Index(+0, +1)),
			Index(index + Index(+1,+1))
		};

		for (int i = 0; i < 8; i++)
		{
			AICostMap[surroundingIndexsLayer1[i]] += 2;
		}
	}
}


void EnemyManager::render()
{
#if DRAW_AI_PATH_COSTMAP
	// Replace all this with the debugRenderText function
	UITextBox::Data textData;
	textData.aligment = "";
	textData.font = "";
	textData.ptSize = 16;
	textData.colour = SDL_Color{ 255, 0, 0 };
	textData.texture = nullptr;
	// to be set
	textData.rect = RectF(); 
	textData.text = "";

	UITextBox text(textData);

	char cost[3];

	for (int y = 0; y < mPathMap.yCount(); y++)
	{
		for (int x = 0; x < mPathMap.xCount(); x++)
		{
			Index index(x, y);
			RectF rect = mPathMap[index].rect();

			if (Camera::Get()->inView(rect) && mPathMap[index].hasCollisionType(MapTile::Floor))
			{
				rect = Camera::Get()->toCameraCoords(rect);

				_itoa_s(mPathMap.costMap()[index], cost, 10);

				text.setText(cost);
				text.setRect(rect);

				text.render();
			}
		}
	}
#endif


	for (Enemy* enemy : mActiveEnemies)
	{
		if (Camera::Get()->inView(enemy->rect()))
		{
			enemy->render();
		}
	}



}


void EnemyManager::clearAllEnemies()
{
	for (std::vector<Enemy*>::iterator iter = mActiveEnemies.begin(); iter != mActiveEnemies.end(); iter++)
	{
		clearAndRemove(iter);
	}

	ASSERT(Warning, mCollisions.isEmpty(), "Enemy colliders are left in the collision trackers when they shouldn't be!\n");
	ASSERT(Warning, mActiveEnemies.size() == 0, "Enemies are left in the active enemy list when they shouldn't be!\n");

	mActiveEnemies.clear();
}


// --- Collisions --- //
std::vector<Collider*> EnemyManager::attackingColliders() const
{
	std::vector<Collider*> colliders;

	for (std::vector<Enemy*>::const_iterator iter = mActiveEnemies.begin(); iter != mActiveEnemies.end(); iter++)
	{
		Enemy* enemy = *iter;
		if (enemy->state() == EnemyState::Attack)
		{
			const EnemyAttack* attackState = static_cast<const EnemyAttack*>(&(enemy->getStateMachine()->getActiveState()));
			if (!attackState->didConnectWithTarget())
			{
				Collider* collider = enemy->collider();
				colliders.push_back(collider);
			}
		}
	}

	return colliders;
}



// --- Private Functions --- //

void EnemyManager::clearAndRemove(std::vector<Enemy*>::iterator& iter)
{
	Enemy* enemy = *iter;
	enemy->clear();

	mCollisions.remove(enemy->collider());
	mPool.returnObject(enemy);

	iter = mActiveEnemies.erase(iter);
}