#include "pch.h"
#include "ArmorAbility.h"

#include "Objects/Actors/Player/Player.h"
#include "Objects/Properties/PropertyBag.h"
#include "Objects/Effects/ArmorEffect.h"

#include "Animations/Animator.h"
#include "Game/Camera.h"


void ArmorAbility::fillValues(ValueMap& values)
{
	mArmor = std::stof(values["Armor"]);
	mMaxDimention = std::stof(values["MaxSize"]);
	mCooldownTime = std::stof(values["Cooldown"]);
}


void ArmorAbility::slowUpdate(float dt)
{
	mAnimator.slowUpdate(dt);
	mRect.SetCenter(mPlayer->position());

	// Completed x animation loops
	if (mAnimator.loops() > 4)
		mAnimator.stop();

	if (hasCooledDown())
		endAbility();
}


void ArmorAbility::activate(Actor* actor)
{
	ArmorEffect* armorEffect = new ArmorEffect(mArmor);
	mPlayer->addEffect(armorEffect);

	Armor* armor = static_cast<Armor*>(mPlayer->getProperty("Armor"));
	SetArmorBarEvent* dataPtr = new SetArmorBarEvent(*armor);
	mEvents.push(EventPacket(dataPtr));

	mAnimator.startAnimation(Action::Active);

	beginCooldown();
}
