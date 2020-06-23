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


void Ability::init(Animator animator, Actor* caster)
{
	mCaster = caster;
	mAnimator = animator;
	mRect.SetSize(realiseSize(animator.frameSize(), mMaxDimention));
}


void Ability::exit()
{
	mAnimator.reset();
	mCooldownTimer.stop();
}


void Ability::render()
{
	if (mState == Running)
	{
		RectF rect = Camera::Get()->toCameraCoords(mRect);
		mAnimator.render(rect);
	}
}


// --- AreaAbility --- //
void AreaAbility::render()
{
	// Range circle
	if (mState == Selected)
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

	Ability::render();
}


Collider AreaAbility::collider()
{
	return Collider(&mRect);
}


bool AreaAbility::isValidTarget(VectorF target, const Map* map)
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
		if (!map->floorCollisionTile(points[i]))
		{
			validPoint = false;
		}
	}

	return validPoint;
}


