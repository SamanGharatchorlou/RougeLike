#include "pch.h"
#include "WeaponCollisionTracker.h"
#include "Colliders/Collider.h"
#include "Weapons/Weapon.h"
#include "Actors/Actor.h"


WeaponCollisionTracker::WeaponCollisionTracker() : mEffectPool(nullptr), mWeapon(nullptr), mTargets(nullptr)
{

}


void WeaponCollisionTracker::checkCollisions()
{
	if (mWeapon->isAttacking())
	{
		std::vector<Collider*> colliders = mWeapon->getColliders();
		for (int j = 0; j < colliders.size(); j++)
		{
			Collider* weaponCollider = colliders[j];

			std::vector<int> overlapIndexes = broadPhaseIndexes(weaponCollider, mTargets);
			for (int i = 0; i < overlapIndexes.size(); i++)
			{
				Actor* target = mTargets->at(overlapIndexes[i]);
				Collider* targetCollider = target->collider();
				if (weaponCollider->doesIntersect(targetCollider))
				{
					if (!mHitTargets.contains(target))
					{
						weaponCollider->setDidHit(true);
						targetCollider->setGotHit(true);

						std::vector<Effect*> effects = mWeapon->getEffects(mEffectPool);

						for (Effect* effect : effects)
						{
							target->addEffect(effect);
						}

						mHitTargets.push(target);
					}
				}
			}
		}
	}
}


void WeaponCollisionTracker::beginAttack()
{
	mHitTargets.clear();
}