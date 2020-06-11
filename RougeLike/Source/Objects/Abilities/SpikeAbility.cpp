#include "pch.h"
#include "SpikeAbility.h"

#include "Objects/Actors/Actor.h"

#include "Graphics/Texture.h"
#include "Animations/Animator.h"
#include "Game/Camera.h"

#include "Objects/Properties/Property.h"
#include "Objects/Effects/KnockbackEffect.h"
#include "Objects/Effects/DamageEffect.h"


void SpikeAbility::fillValues(ValueMap& values)
{
	mDamage = Damage(std::stof(values["Damage"]));
	mForce = std::stof(values["Force"]);
	mMaxDimention = std::stof(values["MaxSize"]);
	mRange = std::stof(values["Range"]);
	mCooldownTime = std::stof(values["Cooldown"]);
}


void SpikeAbility::activate(VectorF position)
{
	mRect.SetBotCenter(position);
	mAnimator.startAnimation(Action::Active);
}


void SpikeAbility::activate(Actor* actor)
{
	KnockbackEffect* knockback = new KnockbackEffect(mRect.Center(), mForce);
	actor->addEffect(knockback);

	DamageEffect* damage = new DamageEffect(mDamage);
	actor->addEffect(damage);

	beginCooldown();
}


void SpikeAbility::slowUpdate(float dt)
{
	mAnimator.slowUpdate(dt);

	// Completed one animation loop
	if (mAnimator.loops() > 0)
		mAnimator.stop();

	if (hasCooledDown())
		endAbility();
}
