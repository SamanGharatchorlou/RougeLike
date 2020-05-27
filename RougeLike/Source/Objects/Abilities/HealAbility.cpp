#include "pch.h"
#include "HealAbility.h"

#include "Objects/Actors/Actor.h"
#include "Objects/Properties/PropertyBag.h"
#include "Objects/Effects/HealEffect.h"
#include "Objects/Attributes/Health.h"

#include "Animations/Animator.h"
#include "Game/Camera.h"


void HealAbility::init(Animator animator)
{
	mAnimator = animator;
	mTimer.start();
}

void HealAbility::slowUpdate(float dt)
{
	mAnimator.slowUpdate(dt);

	// Completed one animation loop
	if (mAnimator.animationIndex() + 1 == mAnimator.animationCount())
		setState(Ability::Finished);
}


void HealAbility::activate(Actor* actor)
{
	mSelf = actor;

	HealEffect* healEffect = new HealEffect(mHeal);
	mSelf->addEffect(healEffect);

	Health* hp = static_cast<Health*>(mSelf->getProperty("Health"));
	SetHealthBarEvent* dataPtr = new SetHealthBarEvent(*hp);
	mEvents.push(EventPacket(dataPtr));

	mAnimator.selectAnimation("activate");
}


void HealAbility::render()
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