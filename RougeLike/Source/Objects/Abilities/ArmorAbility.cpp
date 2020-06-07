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
}


void ArmorAbility::slowUpdate(float dt)
{
	mAnimator.slowUpdate(dt);
	mRect.SetCenter(mPlayer->position());

	// Completed x animation loops
	if (mAnimator.loops() > 4)
		setState(Ability::Finished);
}


void ArmorAbility::activate(Actor* actor)
{
	ArmorEffect* armorEffect = new ArmorEffect(mArmor);
	mPlayer->addEffect(armorEffect);

	Armor* armor = static_cast<Armor*>(mPlayer->getProperty("Armor"));
	SetArmorBarEvent* dataPtr = new SetArmorBarEvent(*armor);
	mEvents.push(EventPacket(dataPtr));

	mAnimator.startAnimation(Action::Active);
}
