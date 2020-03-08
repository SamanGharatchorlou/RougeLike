#include "pch.h"

#include "CollisionTracker.h"
#include "Collider.h"


void CollisionTracker::addCollider(Collider* collider)
{
	mBaseColliders.push_back(collider);
}


void CollisionTracker::subscribe(Collider* collider)
{
#if _DEBUG
	for (auto subbedCollider : mSubbedColliders)
	{
		if (subbedCollider == collider)
		{
			DebugPrint(Log, "Collider is already subscribed with the collider manager\n");
		}
	}
#endif

	mSubbedColliders.push_back(collider);
}


void CollisionTracker::subscribe(std::vector<Collider*> colliders)
{
#if _DEBUG
	for (Collider* collider : colliders)
	{
		for (auto subbedCollider : mSubbedColliders)
		{
			if (subbedCollider == collider)
			{
				DebugPrint(Log, "Collider is already subscribed with the collider manager\n");
			}
		}
#endif

		mSubbedColliders.push_back(collider);
	}
}


void CollisionTracker::clearSubscriptions()
{
	mSubbedColliders.clear();
}



void CollisionTracker::checkBaseCollisions()
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
			}
		}
	}
}


void CollisionTracker::checkAllCollisions()
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

				mSubbedColliders[i]->didCollide(true);
				mSubbedColliders[i]->hasCollidedWith(mBaseColliders[j]);
			}
		}
	}
}