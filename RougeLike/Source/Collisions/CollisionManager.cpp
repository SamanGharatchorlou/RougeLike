#include "pch.h"
#include "CollisionManager.h"


void CollisionManager::processCollisions()
{
	for (int i = 0; i < mCollisionTrackers.size(); i++)
	{
		mCollisionTrackers[i].second.checkCollisions();
	}
}

void CollisionManager::resetColliders()
{
	for (int i = 0; i < mCollisionTrackers.size(); i++)
	{
		mCollisionTrackers[i].second.resetColliders();
	}
}

void CollisionManager::clearColliders()
{
	for (int i = 0; i < mCollisionTrackers.size(); i++)
	{
		mCollisionTrackers[i].second.clearAttackers();
		mCollisionTrackers[i].second.clearDefenders();
	}
}


void CollisionManager::addNewCollisionTracker(Tracker id)
{

	for (int i = 0; i < mCollisionTrackers.size(); i++)
	{
		if (mCollisionTrackers[i].first == id)
		{
			DebugPrint(Log, "A collision tracker with the id: %d already exists\n", id);
			return;
		}
	}

	CollisionTracker collisionTracker;
	std::pair<Tracker, CollisionTracker> collisionTrackerInfo(id, collisionTracker);
	mCollisionTrackers.push_back(collisionTrackerInfo);
}


void CollisionManager::addAttackers(Tracker id, std::vector<Collider*> attackers)
{
	CollisionTracker* tracker = getTracker(id);

	if (tracker)
		tracker->addAttackers(attackers);
}


void CollisionManager::addDefenders(Tracker id, std::vector<Collider*> defenders)
{
	CollisionTracker* tracker = getTracker(id);

	if (tracker)
		tracker->addDefenders(defenders);
}


void CollisionManager::removeDefender(Tracker id, Collider* defender)
{
	CollisionTracker* tracker = getTracker(id);

	if(tracker)
		tracker->removeDefender(defender);
}


void CollisionManager::removeAllAttackers(Tracker id)
{
	CollisionTracker* tracker = getTracker(id);

	if (tracker)
		tracker->clearAttackers();
}


void CollisionManager::removeAllDefenders(Tracker id)
{
	CollisionTracker* tracker = getTracker(id);

	if (tracker)
		tracker->clearDefenders();
}


CollisionTracker* CollisionManager::getTracker(Tracker id)
{
	for (int i = 0; i < mCollisionTrackers.size(); i++)
	{
		if (mCollisionTrackers[i].first == id)
		{
			return &mCollisionTrackers[i].second;
		}
	}

	DebugPrint(Log, "No collision tracker with the id: %d exists.\n", id);
	return nullptr;
}