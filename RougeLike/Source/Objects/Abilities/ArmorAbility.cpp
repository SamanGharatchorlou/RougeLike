#include "pch.h"
#include "ArmorAbility.h"

#include "Objects/Actors/Actor.h"
#include "Objects/Properties/PropertyBag.h"
#include "Objects/Effects/ArmorEffect.h"

#include "Animations/Animator.h"
#include "Game/Camera.h"


void ArmorAbility::init(Animator animator)
{
	mAnimator = animator;
}

void ArmorAbility::slowUpdate(float dt)
{
	mAnimator.slowUpdate(dt);

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


void ArmorAbility::render()
{
	// Size
	VectorF baseDimentions = mAnimator.getSpriteTile()->getRect().Size();
	VectorF size = baseDimentions * 1.4f;
	RectF rect(VectorF(), size);

	// Center position
	VectorF selfPosition = Camera::Get()->toCameraCoords(mSelf->position());
	rect.SetCenter(selfPosition);

	mAnimator.getSpriteTile()->render(rect);
}