#include "pch.h"
#include "LightningAbility.h"

#include "Game/Camera/Camera.h"
#include "Input/InputManager.h"
#include "Actors/Actor.h"


#include "Objects/Pools/EffectPool.h"


void LightningAbility::init()
{
	// Set a small size, we don't want to select the wrong enemy
	mRect.SetSize(VectorF(5.0f, 5.0f));

	mBounceTime = 0.25f;

	RangedAbility::init();

	mAnimator.selectAnimation(Animation::Active);
	mAnimator.start();

	mFirstPass = true;
	mCountingLoop = false;
}


void LightningAbility::handleInput(const InputManager* input)
{
	VectorF cameraPosition = Camera::Get()->rect().TopLeft();
	VectorF worldCursorPosition = input->cursorPosition() + cameraPosition;

	mRect.SetCenter(worldCursorPosition);
}


void LightningAbility::activate()
{
	mActivateCollisions = true;

	// Cursor position - as updated by handleInput
	mRect.SetSize(25.0f, 25.0f);

	mTimer.start();
	mAnimator.start();
}


bool LightningAbility::activateOn(Actor* target, EffectPool* effectPool)
{
	if (mCountingLoop)
	{
		// build a list of all possible targets
		mTargetList.push_back(target);
	}
	else
	{
		if (mNextTarget == target)
		{
			mHitList.insert(target);
			applyEffects(target, effectPool);

			mBounces++;
			mRect.SetCenter(target->position());

			mActivateCollisions = false;
			mTimer.restart();
			return true;
		}
	}

	return false;
}



void LightningAbility::fastUpdate(float dt)
{
	if (mActivateCollisions)
	{
		// First pass we just need to get all the possible targets (in activateOn)
		if (mFirstPass)
		{
			mNextTarget = nullptr;
			mCountingLoop = true;
			mFirstPass = false;

			mRect.SetSize(200.0f, 200.0f);

			printf("first pass\n");
		}
		// Next pass we can use this target list to get the next target
		else if(!mNextTarget)
		{
			mCountingLoop = false;

			mNextTarget = getNextRandomTarget();

			printf("get next target\n");

			// If we dont find a target that means there's nothing we can chain to, end the ability
			if (!mNextTarget)
			{
				printf("no valid next target\n");
				mCompleted = true;
				mActivateCollisions = false;
			}
		}
	}
}


void LightningAbility::slowUpdate(float dt)
{
	mAnimator.slowUpdate(dt);

	if (mTimer.getSeconds() > mBounceTime)
	{
		mActivateCollisions = true;
		mFirstPass = true;
	}

	if (mTimer.getSeconds() > 1.0f || mBounces > 5)
	{
		mCompleted = true;
	}

}


void LightningAbility::render()
{
	if (mState == AbilityState::Running)
	{
		if (mHitList.size() == 1)
		{
			renderBoltBetweenTargets(mCaster, *mHitList.begin());
		}
		else if (mHitList.size() > 1)
		{
			for (std::unordered_set<Actor*>::iterator iter = mHitList.begin(); iter != mHitList.end(); iter++)
			{
				std::unordered_set<Actor*>::iterator nextIter = (std::next(iter, 1));
				if (nextIter == mHitList.end())
					break;

				renderBoltBetweenTargets(*iter, *nextIter);
			}
		}
	}

#if DRAW_ABILITY_RECTS
	debugDrawRect(mRect, RenderColour::Yellow);
#endif
#if TRACK_COLLISIONS
	if (mCollider)
		mCollider->renderCollider();
#endif
}


void LightningAbility::renderBoltBetweenTargets(const Actor* actorA, const Actor* actorB) const
{
	// make sure targets havent died and been wiped by the enemy manager during the animation
	// There must be a nicer way to figure out if an actor is dead or not...
	if (!actorA->rect().TopLeft().isZero() && !actorB->rect().TopLeft().isZero())
	{
		VectorF caster_target = actorB->position() - actorA->position();
		float length = std::sqrt(caster_target.magnitudeSquared());

		RectF rect;
		rect.SetSize(VectorF(length, length));
		rect.SetLeftCenter(actorA->position());

		double angle = getRotation(caster_target) - 90.0f;
		VectorF aboutPoint(0.0f, length / 2.0f);

		rect = Camera::Get()->toCameraCoords(rect);
		mAnimator.render(rect, angle, aboutPoint);
	}
}


void LightningAbility::applyEffects(Actor* actor, EffectPool* effectPool)
{
	applyEffect(EffectType::Damage, actor, effectPool);
	applyEffect(EffectType::Stun, actor, effectPool);
}


void LightningAbility::exit()
{
	mTimer.stop();
	mHitList.clear();

	mRect.SetSize(25.0f, 25.0f);

	mNextTarget = nullptr;
	mTargetList.clear();
	mFirstPass = true;
	mBounces = 0;
}




Actor* LightningAbility::getNextRandomTarget()
{
	Actor* nextRandomTarget = nullptr;

	while (mTargetList.size() > 0)
	{
		int randomTargetIndex = rand() % mTargetList.size();

		Actor* randomTarget = mTargetList[randomTargetIndex];

		// target has already been hit, remove from target list
		if (mHitList.count(randomTarget))
		{
			for (std::vector<Actor*>::iterator iter = mTargetList.begin(); iter != mTargetList.end(); iter++)
			{
				if (randomTarget == *iter)
				{
					mTargetList.erase(iter);
					break;
				}
			}

			// we dont have a valid target yet, so continue to next loop
			continue;
		}

		// we have a valid target
		nextRandomTarget = randomTarget;
		break;
	}

	return nextRandomTarget;
}