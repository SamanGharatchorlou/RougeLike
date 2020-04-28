#include "pch.h"

#include "CollisionTracker.h"
#include "Collider.h"


void CollisionTracker::addAttacker(Collider* collider)
{
#if _DEBUG
	for (auto attacker : mAttackers)
	{
		if (collider == attacker)
		{
			DebugPrint(Log, "Attacking collider is already subscribed with the collider manager\n");
		}
	}
#endif

	mAttackers.push_back(collider);
}


void CollisionTracker::addAttackers(std::vector<Collider*> colliders)
{
	for (Collider* attacker : colliders)
	{
		addAttacker(attacker);
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


void CollisionTracker::removeDefender(Collider* collider)
{
	for (std::vector<Collider*>::iterator iter = mDefenders.begin(); iter != mDefenders.end(); iter++)
	{
		if (collider == *iter)
		{
			mDefenders.erase(iter);
			return;
		}
	}

	DebugPrint(Log, "Attempting to remove collider from defending collider list where it is not contained\n");
}


void CollisionTracker::clearAttackers()
{
	mAttackers.clear();
}



void CollisionTracker::checkAttackerCollisions()
{
	for (unsigned int j = 0; j < mAttackers.size(); j++)
	{
		for (unsigned int i = 0; i < mDefenders.size(); i++)
		{
			// check pixle collision
			if (mAttackers[j]->doesIntersect(mDefenders[i]))
			{
				mAttackers[j]->hasCollidedWith(mDefenders[i]);
				mAttackers[j]->setDidHit(true);
			}
		}
	}
}



void CollisionTracker::checkDefenderCollisions()
{
	for (unsigned int j = 0; j < mAttackers.size(); j++)
	{
		for (unsigned int i = 0; i < mDefenders.size(); i++)
		{
			// check pixle collision
			if (mAttackers[j]->doesIntersect(mDefenders[i]))
			{
				// Defender
				mDefenders[i]->setGotHit(true);
				mDefenders[i]->hasCollidedWith(mAttackers[j]);
			}
		}
	}
}




void CollisionTracker::checkAttackerDefenderCollisions()
{
	for (unsigned int j = 0; j < mAttackers.size(); j++)
	{
		for (unsigned int i = 0; i < mDefenders.size(); i++)
		{
			// check pixle collision
			if (mAttackers[j]->doesIntersect(mDefenders[i]))
			{
				// Attacker
				mAttackers[j]->setDidHit(true);
				mAttackers[j]->hasCollidedWith(mDefenders[i]);

				// Defender
				mDefenders[i]->setGotHit(true);
				mDefenders[i]->hasCollidedWith(mAttackers[j]);
			}
		}
	}
}