#include "pch.h"
#include "Ability.h"

#include "Graphics/Texture.h"

#include "Game/Camera.h"
#include "Map/Map.h"
#include "Objects/Actors/Player/Player.h"

#include "Events/Events.h"
#include "Graphics/RenderManager.h"


// --- Ability --- //
EventPacket Ability::popEvent()
{
	ASSERT(Error, mEvents.size() > 0, "Ability has no event when attempting to pop one.\n");
	EventPacket event = mEvents.front();
	mEvents.pop();
	return event;
}


void Ability::fillAbilityValues(ValueMap& values)
{
	mMaxDimention = std::stof(values["MaxSize"]);
	mCooldown = Cooldown(std::stof(values["Cooldown"]));
}


void Ability::init(Animator animator, Actor* caster)
{
	mCaster = caster;
	mAnimator = animator;
	mRect.SetSize(realiseSize(animator.frameSize(), mMaxDimention));
}


void Ability::exit()
{
	mAnimator.reset();
	mCooldown.stop();
}


// --- Range ---
void RangedAbility::fillRangedAbilityValues(ValueMap& values)
{
	mRange = std::stof(values["Range"]);
}


void RangedAbility::renderRangeCircle()
{
	VectorF position = Camera::Get()->toCameraCoords(mCaster->position());
	VectorF size = VectorF(mRange, mRange) * 2.0f;

	RectF rect = RectF(VectorF(), size);
	rect.SetCenter(position);

	mRangeCircle->render(rect);

	RenderEvent* event = new RenderEvent(mRangeCircle, rect, (int)RenderLayer::Floor);
	EventPacket ep(event);
	mEvents.push(ep);
}


Collider RangedAbility::collider()
{
	return Collider(&mRect);
}


bool RangedAbility::isValidTarget(VectorF target, const Map* map)
{
	// Is it in range
	if (distanceSquared(mCaster->position(), target) > mRange * mRange)
		return false;

	// Is the target position a floor tile
	RectF rect = mRect.MoveCopy(target);
	VectorF points[4]{ rect.TopLeft(), rect.TopRight(), rect.BotRight(), rect.BotLeft() };

	bool validPoint = true;

	for (int i = 0; i < 4; i++)
	{
		if (map->collisionType(points[i]) != CollisionTile::Floor)
			validPoint = false;
	}

	return validPoint;
}


// -- SELF
void TargetSelfAbility::render()
{
	if (mState == Running)
	{
		RectF rect = Camera::Get()->toCameraCoords(mRect);
		mAnimator.render(rect);
	}
}


// -- Position
void TargetPositionAbility::render()
{
	if (mState == Selected)
	{
		renderRangeCircle();
	}
	else if (mState == Running)
	{
		RectF rect = Camera::Get()->toCameraCoords(mRect);
		mAnimator.render(rect);
	}
}


// -- Attack
void TargePositionAttackAbility::render()
{
	if (mState == Selected)
	{
		renderRangeCircle();
	}
	else if (mState == Running)
	{
		RectF rect = Camera::Get()->toCameraCoords(mRect);
		mAnimator.render(rect);
	}
}


void TargePositionAttackAbility::sendActivateOnRequest()
{
	ActivateAreaAttack* smashEvent = new ActivateAreaAttack(this);
	mEvents.push(EventPacket(smashEvent));
}


