#include "pch.h"
#include "ArmorAbility.h"

#include "Objects/Actors/Actor.h"
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
	mRect.SetCenter(mSelf->position());

	if (mTimer.getSeconds() > 2.0f)
		setState(Ability::Finished);
}


void ArmorAbility::activate(Actor* actor)
{
	mSelf = actor;

	ArmorEffect* armorEffect = new ArmorEffect(mArmor);
	mSelf->addEffect(armorEffect);

	Armor* armor = static_cast<Armor*>(mSelf->getProperty("Armor"));
	SetArmorBarEvent* dataPtr = new SetArmorBarEvent(*armor);
	mEvents.push(EventPacket(dataPtr));

	mTimer.restart();
	mAnimator.selectAnimation("activate");
}
