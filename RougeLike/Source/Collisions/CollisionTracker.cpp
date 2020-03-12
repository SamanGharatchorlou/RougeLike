#include "pch.h"

#include "CollisionTracker.h"
#include "Collider.h"


void CollisionTracker::addCollider(Collider* collider)
{
	mAttackers.push_back(collider);
}


void CollisionTracker::subscribe(Collider* collider)
{
#if _DEBUG
	for (auto subbedCollider : mDefenders)
	{
		if (subbedCollider == collider)
		{
			DebugPrint(Log, "Collider is already subscribed with the collider manager\n");
		}
	}
#endif

	mDefenders.push_back(collider);
}


void CollisionTracker::subscribe(std::vector<Collider*> colliders)
{
	for (Collider* collider : colliders)
	{
#if _DEBUG
		for (auto subbedCollider : mDefenders)
		{
			if (subbedCollider == collider)
			{
				DebugPrint(Log, "Collider is already subscribed with the collider manager\n");
			}
		}
#endif

		mDefenders.push_back(collider);
	}
}


void CollisionTracker::addAttackers(std::vector<Collider*> colliders)
{
	for (Collider* attacker : colliders)
	{
#if _DEBUG
		for (auto attackingCollider : mAttackers)
		{
			if (attacker == attackingCollider)
			{
				DebugPrint(Log, "Attacking collider is already subscribed with the collider manager\n");
			}
		}
#endif

		mAttackers.push_back(attacker);
	}
}


void CollisionTracker::addDefender(Collider* collider)
{
#if _DEBUG
	for (auto defender : mDefenders)
	{
		if (collider == defender)
		{
			DebugPrint(Log, "Defending collider is already subscribed with the collider manager\n");
		}
	}
#endif

	mDefenders.push_back(collider);
}


void CollisionTracker::clearSubscriptions()
{
	mDefenders.clear();
}


void CollisionTracker::clearAttackers()
{
	mAttackers.clear();
}



void CollisionTracker::checkBaseCollisions()
{
	for (unsigned int j = 0; j < mAttackers.size(); j++)
	{
		if (!mAttackers[j]->isActive())
			continue;

		for (unsigned int i = 0; i < mDefenders.size(); i++)
		{
			if (!mDefenders[i]->isActive())
				continue;

			// check pixle collision
			if (mAttackers[j]->doesIntersect(mDefenders[i]))
			{
				mAttackers[j]->hasCollidedWith(mDefenders[i]);
				mAttackers[j]->didCollide(true);
			}
		}
	}
}


void CollisionTracker::checkAllCollisions()
{
	for (unsigned int j = 0; j < mAttackers.size(); j++)
	{
		if (!mAttackers[j]->isActive())
			continue;

		for (unsigned int i = 0; i < mDefenders.size(); i++)
		{
			if (!mDefenders[i]->isActive())
				continue;

			// check pixle collision
			if (mAttackers[j]->doesIntersect(mDefenders[i]))
			{
				mAttackers[j]->hasCollidedWith(mDefenders[i]);
				mAttackers[j]->didCollide(true);

				//mAttackers[i]->didCollide(true);
				//mAttackers[i]->hasCollidedWith(mDefenders[j]);
			}
		}
	}
}


// TEMP
void CollisionTracker::checkCollisions()
{
	for (unsigned int j = 0; j < mAttackers.size(); j++)
	{
		if (!mAttackers[j]->isActive())
			continue;

		for (unsigned int i = 0; i < mDefenders.size(); i++)
		{
			if (!mDefenders[i]->isActive())
				continue;

			// check pixle collision
			if (mAttackers[j]->doesIntersect(mDefenders[i]))
			{
				mAttackers[j]->hasCollidedWith(mDefenders[i]);
				mAttackers[j]->setDidHit(true);

				// TODO: do I need to split out this has collided with to got hit by and did hit versions?
				// unless its being hit and doing the hitting ain the same frame.. unlikely?
				mDefenders[i]->setGotHit(true);
				mDefenders[i]->hasCollidedWith(mAttackers[j]);
			}
		}
	}
}