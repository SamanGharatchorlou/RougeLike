#include "pch.h"
#include "WeaponCollisionManager.h"

#include "CollisionTracker.h"
#include "CollisionManager.h"

#include "PlayerCollisions.h"
#include "Actors/Player/Player.h"

#include "EffectCollider.h"
#include "Weapons/Melee/MeleeWeapon.h"

#include "Objects/Effects/EffectTypes/DamageEffect.h"
#include "Objects/Effects/EffectTypes/DisplacementEffect.h"


void WeaponCollisionManager::init(Player* player, CollisionTracker* tracker)
{
	mPlayer = player;
	mTracker = tracker;
}


void WeaponCollisionManager::processWeaponEffects()
{
	std::vector<EffectCollider*> weaponColliders = mPlayer->weapon()->getEffectColliders();
	
	for (int i = 0; i < weaponColliders.size(); i++)
	{
		if(!weaponColliders[i]->hasEffects())
			addWeaponEffects(weaponColliders[i]);

		updateWeaponEffect(weaponColliders[i]);
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
void WeaponCollisionManager::addWeaponEffects(EffectCollider* weaponCollider)
{
	const MeleeWeaponData* data = mPlayer->weapon()->getData();

	Effect* dmgEffect = mPlayer->getEffectFromPool(EffectType::Damage);
	DamageEffect* damageEffect = static_cast<DamageEffect*>(dmgEffect);
	damageEffect->set(data->damage);
	weaponCollider->addEffect(damageEffect);

	Effect* displaceEffect = mPlayer->getEffectFromPool(EffectType::Displacement);
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

void WeaponCollisionManager::addEnemiesToExcludedList()
{
	std::vector<Collider*> enemies = mTracker->defenders();

	for (int i = 0; i < enemies.size(); i++)
	{
		if (enemies[i]->gotHit())
		{
			mTracker->addExcludedDefender(enemies[i]);
		}
	}
}
