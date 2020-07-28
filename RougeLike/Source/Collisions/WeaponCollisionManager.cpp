#include "pch.h"
#include "WeaponCollisionManager.h"

#include "CollisionTracker.h"
#include "Objects/Effects/EffectPool.h"

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
				addWeaponEffects(weaponColliders[i], effects);


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

		if (effect->type() == EffectType::Displacement)
		{
			DisplacementEffect* displacementEffect = static_cast<DisplacementEffect*>(effect);
			displacementEffect->update(mPlayer->position());
		}

		weaponCollider->addEffect(effect);
	}
}


// top up weapon effects, ensure its always full
void WeaponCollisionManager::addWeaponEffects(EffectCollider* weaponCollider, EffectPool* effects)
{
	const MeleeWeaponData* data = mPlayer->weapon()->getData();

	Effect* dmgEffect = effects->getObject(EffectType::Damage);
	DamageEffect* damageEffect = static_cast<DamageEffect*>(dmgEffect);
	damageEffect->set(data->damage);
	weaponCollider->addEffect(damageEffect);

	Effect* displaceEffect = effects->getObject(EffectType::Displacement);
	DisplacementEffect* displacementEffect = static_cast<DisplacementEffect*>(displaceEffect);

	VectorF source; // to be updated
	float force = data->knockbackForce;
	float distance = data->knockbackDistance;
	displacementEffect->set(source, force, distance);
	weaponCollider->addEffect(displacementEffect);
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
				// TODO: does this ever trigger, does the comparison correctly work?
				mTracker->addExcludedDefender(collider);
			}
		}
	}
}
