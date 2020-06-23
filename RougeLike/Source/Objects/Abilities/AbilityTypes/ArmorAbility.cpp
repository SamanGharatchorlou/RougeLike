#include "pch.h"
#include "ArmorAbility.h"

#include "Objects/Actors/Player/Player.h"
#include "Objects/Effects/ArmorEffect.h"
#include "Objects/Effects/EffectPool.h"

#include "Animations/Animator.h" 


void ArmorAbility::fillValues(ValueMap& values)
{
	mArmor = Armor(std::stof(values["Armor"]));
	mMaxDimention = std::stof(values["MaxSize"]);
	mCooldownTime = std::stof(values["Cooldown"]);
}


void ArmorAbility::slowUpdate(float dt)
{
	mAnimator.slowUpdate(dt);
	mRect.SetCenter(mCaster->position());

	// Completed x animation loops
	if (mAnimator.loops() > 4)
		mAnimator.stop();

	if (hasCooledDown())
		endAbility();
}


void ArmorAbility::activate(Actor* actor, EffectPool* effectPool)
{
	mAnimator.startAnimation(Action::Active);

	Effect* effect = effectPool->getEffect(EffectType::Armor);
	ArmorEffect* armorEffect = static_cast<ArmorEffect*>(effect);
	armorEffect->set(mArmor);
	actor->addEffect(effect);

	// TODO: Play specific, remove it...
	static_cast<Player*>(actor)->updateUI();
	beginCooldown();
}
