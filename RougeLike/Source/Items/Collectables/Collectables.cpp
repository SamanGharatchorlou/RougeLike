#include "pch.h"
#include "Collectables.h"

#include "Collectable.h"

#include "Game/Camera/Camera.h"
#include "Game/Data/GameData.h"
#include "Collisions/CollisionManager.h"

#if _DEBUG
#include "Debug/DebugDraw.h"
#endif


Collectables::~Collectables()
{
	clear();
}

void Collectables::clear()
{
	mCollector = nullptr;
	mCollisions = nullptr;

	// WARNING: need to remove from collisions first???
	for (int i = 0; i < mCollectables.size(); i++)
	{
		mBuilder.returnCollectable(mCollectables[i]);
		mCollectables[i] = nullptr;
	}

	mCollectables.clear();
	mBuilder.clear();
}


void Collectables::spawn(const XMLNode collectablesNode, const Map* map)
{
	std::queue<CollectableSpawner::SpawnData> dataQueue = mSpawner.getSpawnList(collectablesNode, map);

	while (dataQueue.size() > 0)
	{
		CollectableSpawner::SpawnData data = dataQueue.front();
		dataQueue.pop();

		Collectable* collectable = mBuilder.build(data);
		mCollectables.push_back(collectable);
		mCollisions->addDefender(CollisionManager::Player_Hit_Collectable, collectable->collider());
	}
}


void Collectables::init(CollisionManager* collisions, PlayerManager* collector)
{
	mCollisions = collisions;
	mCollector = collector;
}


void Collectables::load()
{
	mBuilder.loadPool();
}


void Collectables::slowUpdate(float dt)
{
	std::vector<Collectable*>::iterator iter;
	for (iter = mCollectables.begin(); iter != mCollectables.end(); )
	{
		Collectable* collectable = *iter;

		if (collectable->pickedUp())
		{
			collectable->activate(mCollector);

			iter = mCollectables.erase(iter);
			mCollisions->removeDefender(CollisionManager::Player_Hit_Collectable, collectable->collider());

			mBuilder.returnCollectable(collectable);
		}
		else
		{
			iter++;
		}
	}
}


void Collectables::render()
{
	for (unsigned int i = 0; i < mCollectables.size(); i++)
	{
		RectF worldRect = mCollectables[i]->rect();

		if (Camera::Get()->inView(worldRect))
		{
			RectF cameraRect = Camera::Get()->toCameraCoords(worldRect);
#if DRAW_COLLECTABLE_RECT
			debugDrawRect(mCollectables[i]->colliderRect(), RenderColour::Blue);
			debugDrawRect(worldRect, RenderColour::Green);
#endif
			mCollectables[i]->render(cameraRect);
		}
	}
}

