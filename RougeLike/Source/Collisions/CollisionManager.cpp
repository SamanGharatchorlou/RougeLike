#include "pch.h"
#include "CollisionManager.h"


CollisionManager::~CollisionManager()
{
	for (TrackerMap::iterator iter = mTrackers.begin(); iter != mTrackers.end(); iter++)
	{
		delete iter->second;
		iter->second = nullptr;
	}
}


void CollisionManager::fastUpdate()
{
	for (TrackerMap::iterator iter = mTrackers.begin(); iter != mTrackers.end(); iter++)
	{
		iter->second->checkCollisions();
	}
}


void CollisionManager::resetColliders()
{
	for (TrackerMap::iterator iter = mTrackers.begin(); iter != mTrackers.end(); iter++)
	{
		iter->second->resetColliders();
	}
}


void CollisionManager::clearColliders()
{
	for (TrackerMap::iterator iter = mTrackers.begin(); iter != mTrackers.end(); iter++)
	{
		iter->second->clearAttackers();
		iter->second->clearDefenders();
	}
}


void CollisionManager::addBasicCollisionTracker(Tracker id)
{
	for (TrackerMap::iterator iter = mTrackers.begin(); iter != mTrackers.end(); iter++)
	{
		if (iter->first == id)
		{
			DebugPrint(Log, "A collision tracker with the id: %d already exists\n", id);
			return;
		}
	}

	CollisionTracker* tracker = new CollisionTracker;
	mTrackers[id] = tracker;
}


void CollisionManager::addComplexCollisionTracker(Tracker id)
{
	for (TrackerMap::iterator iter = mTrackers.begin(); iter != mTrackers.end(); iter++)
	{
		if (iter->first == id)
		{
			DebugPrint(Log, "A collision tracker with the id: %d already exists\n", id);
			return;
		}
	}

	CollisionTracker* tracker = new ComplexCollisionTracker;
	mTrackers[id] = tracker;
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
	for (TrackerMap::iterator iter = mTrackers.begin(); iter != mTrackers.end(); iter++)
	{
		if (iter->first == id)
		{
			return iter->second;
		}
	}

	DebugPrint(Log, "No collision tracker with the id: %d exists.\n", id);
	return nullptr;
}
