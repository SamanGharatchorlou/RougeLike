#include "pch.h"
#include "WeaponCollisionManager.h"

#include "CollisionTracker.h"
#include "Objects/Pools/EffectPool.h"

#include "Actors/Player/Player.h"
#include "Weapons/Melee/MeleeWeapon.h"
#include "Weapons/WeaponData.h"

#include "Objects/Effects/EffectTypes/DamageEffect.h"
#include "Objects/Effects/EffectTypes/DisplacementEffect.h"


void WeaponCollisionManager::init(Player* player, CollisionTracker* tracker)
{
	mPlayer = player;
	mTracker = tracker;
}


void WeaponCollisionManager::processWeaponEffects()
{
	if (mPlayer->weapon()->didHit())
	{
		std::vector<EffectCollider*> weaponColliders = mPlayer->weapon()->getEffectColliders();

		for (int i = 0; i < weaponColliders.size(); i++)
		{
			EffectCollider* collider = weaponColliders[i];

			if (!collider->hasEffects())
			{
				UniqueQueue<Effect*> effects = mPlayer->effects().getNewAttackingEffects();
				collider->addNewEffects(effects);
			}
			
			VectorF position = mPlayer->position();
			mPlayer->effects().attackingData().addXYPosition(position);
			collider->updateEffectData(mPlayer->effects().attackingData());
		}
	}
}


void WeaponCollisionManager::clearExcludedList()
{
	mTracker->clearExcludedDefenders();
}

void WeaponCollisionManager::addCollidersToExcludedList()
{
	std::vector<Collider*> colliders = mTracker->defenders();

	for (int i = 0; i < colliders.size(); i++)
	{
		Collider* collider = colliders[i];
		if (collider->gotHit())
		{
			Collider* hitByCollider = collider->getOtherCollider();
			if (mPlayer->weapon()->containsCollider(hitByCollider))
			{
				mTracker->addExcludedDefender(collider);
			}
		}
	}
}
