#include "pch.h"
#include "ArmorAbility.h"

#include "Objects/Effects/ArmorEffect.h"
#include "Objects/Effects/EffectPool.h"

#include "Animations/Animator.h"
#include "Objects/Actors/Player/Player.h"
#include "Objects/Properties/PropertyBag.h"


void ArmorAbility::fillValues(ValueMap& values)
{
	mArmor = Armor(std::stof(values["Armor"]));
	mMaxDimention = std::stof(values["MaxSize"]);
	mCooldownTime = std::stof(values["Cooldown"]);
}


void ArmorAbility::slowUpdate(float dt)
{
	mAnimator.slowUpdate(dt);
	mRect.SetCenter(mPlayer->position()); // TODO: Can i remove the mPlayer from ability.h ? this is set on activate...

	// Completed x animation loops
	if (mAnimator.loops() > 4)
		mAnimator.stop();

	if (hasCooledDown())
		endAbility();
}


void ArmorAbility::activate(Actor* actor, EffectPool* effectPool)
{
	Effect* effect = effectPool->getEffect(EffectType::Armor);
	ArmorEffect* armorEffect = static_cast<ArmorEffect*>(effect);
	armorEffect->set(mArmor);
	actor->addEffect(effect);

	// TODO: move this to somewhere else
	Armor* armor = static_cast<Armor*>(mPlayer->getProperty("Armor"));
	SetArmorBarEvent* dataPtr = new SetArmorBarEvent(*armor);
	mEvents.push(EventPacket(dataPtr));

	mAnimator.startAnimation(Action::Active);

	beginCooldown();
}
