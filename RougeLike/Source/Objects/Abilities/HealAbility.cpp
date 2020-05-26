#include "pch.h"
#include "HealAbility.h"

#include "Objects/Actors/Actor.h"
#include "Objects/Properties/PropertyBag.h"
#include "Objects/Effects/HealEffect.h"
#include "Objects/Attributes/Health.h"

#include "Graphics/Texture.h"
#include "Animations/Animator.h"
#include "Game/Camera.h"


HealAbility::HealAbility(float heal) : mHeal(heal)
{
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
	VectorF selfSize = mSelf->rect().Size();
	VectorF baseDimentions = mAnimator.getSpriteTile()->getRect().Size();

	float scale = selfSize.y * 1.4f / baseDimentions.y;
	VectorF size = baseDimentions * scale;

	// Center position
	VectorF selfPosition = Camera::Get()->toCameraCoords(mSelf->position());
	VectorF position = selfPosition - (size / 2.0f);
	RectF rect(position, size);
	mAnimator.getSpriteTile()->render(rect);
}