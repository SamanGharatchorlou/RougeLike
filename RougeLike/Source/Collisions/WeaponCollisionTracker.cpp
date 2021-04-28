#include "pch.h"
#include "WeaponCollisionTracker.h"
#include "Colliders/Collider.h"
#include "Weapons/Weapon.h"
#include "Actors/Actor.h"

void WeaponCollisionTracker::checkCollisions()
{
	if (mWeapon->isAttacking())
	{
		Collider* weaponCollider = mWeapon->getCollider();

		std::vector<int> overlapIndexes = broadPhaseIndexes(weaponCollider, mTargets);
		for (int i = 0; i < overlapIndexes.size(); i++)
		{
			Actor* target = mTargets->at(overlapIndexes[i]);
			Collider* targetCollider = target->collider();
			if (weaponCollider->doesIntersect(targetCollider))
			{
				weaponCollider->setDidHit(true);
				targetCollider->setGotHit(true);

				if (!mHitTargets.contains(target))
				{
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


void WeaponCollisionTracker::beginAttack()
{
	mHitTargets.clear();
}