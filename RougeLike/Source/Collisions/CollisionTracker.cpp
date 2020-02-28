#include "pch.h"

#include "CollisionTracker.h"
#include "Collider.h"


void CollisionTracker::addCollider(Collider* collider)
{
	mBaseColliders.push_back(collider);
}


void CollisionTracker::subscribe(Collider* collider)
{
	for (auto subbedCollider : mSubbedColliders)
	{
		if (subbedCollider == collider)
		{
			DebugPrint(Log, "Collider is already subscribed with the collider manager");
		}
	}

	mSubbedColliders.push_back(collider);
}


void CollisionTracker::subscribe(std::vector<Collider*> colliders)
{
	for (Collider* collider : colliders)
	{
		for (auto subbedCollider : mSubbedColliders)
		{
			if (subbedCollider == collider)
			{
				DebugPrint(Log, "Collider is already subscribed with the collider manager");
			}
		}

		mSubbedColliders.push_back(collider);
	}
}


void CollisionTracker::clearSubscriptions()
{
	mSubbedColliders.clear();
}



void CollisionTracker::checkForCollisions()
{
	for (unsigned int j = 0; j < mBaseColliders.size(); j++)
	{
		if (!mBaseColliders[j]->isActive())
			continue;

		for (unsigned int i = 0; i < mSubbedColliders.size(); i++)
		{
			if (!mSubbedColliders[i]->isActive())
				continue;

			// check pixle collision
			if (mBaseColliders[j]->doesIntersect(mSubbedColliders[i]))
			{
				mBaseColliders[j]->hasCollidedWith(mSubbedColliders[i]);
				mBaseColliders[j]->didCollide(true);

				// TODO: make sure this doesnt break anything else
				//mSubbedColliders[i]->didCollide(true);
			}
		}
	}
}