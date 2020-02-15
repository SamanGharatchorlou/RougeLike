#include "pch.h"

#include "ColliderManager.h"
#include "Collider.h"


void ColliderManager::addCollider(Collider* collider)
{
	mBaseColliders.push_back(collider);
}


void ColliderManager::subscribe(Collider* collider)
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


void ColliderManager::update()
{
	for (unsigned int j = 0; j < mBaseColliders.size(); j++)
	{
		if (!mBaseColliders[j]->isActive())
			continue;

		for (int i = 0; i < mSubbedColliders.size(); i++)
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
