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


void WeaponCollisionManager::processWeaponEffects(EffectPool* effects)
{
	if (mPlayer->weapon()->didHit())
	{
		std::vector<EffectCollider*> weaponColliders = mPlayer->weapon()->getEffectColliders();

		for (int i = 0; i < weaponColliders.size(); i++)
		{
			if (!weaponColliders[i]->hasEffects())
			{
				addWeaponEffects(weaponColliders[i]);
			}
				
			// this will trigger as soon as I attack, only if I didHit()?
			updateWeaponEffect(weaponColliders[i]);
		}
	}
}


void WeaponCollisionManager::updateWeaponEffect(EffectCollider* weaponCollider)
{
	for (int i = 0; i < weaponCollider->effectCount(); i++)
	{
		Effect* effect = weaponCollider->popEffect();

		// TODO: replace this with the property map system
		if (effect->type() == EffectType::Displacement)
		{
			DisplacementEffect* displacementEffect = static_cast<DisplacementEffect*>(effect);
			displacementEffect->update(mPlayer->position());
		}

		weaponCollider->addEffect(effect);
	}
}


// top up weapon effects, ensure its always full
void WeaponCollisionManager::addWeaponEffects(EffectCollider* weaponCollider)
{
	std::queue<Effect*> effects = mPlayer->effects().getAttackingEffects();

	while (effects.size() > 0)
	{
		Effect* effect = effects.front();
		effects.pop();

		weaponCollider->addEffect(effect);
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
