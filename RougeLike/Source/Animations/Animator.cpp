#include "pch.h"
#include "Animator.h"

#include "Graphics/Texture.h"


Animator::Animator() : mActiveIndex(0), speedFactor(1.0f), mFrameTime(0.0f), mState(TimeState::Stopped) { }


void Animator::addAnimation(const Animation::Data& data)
{
	Animation animation(data);
	mAnimations.push_back(animation);
}

void Animator::render(RectF rect, SDL_RendererFlip flip) const
{
	if(isStarted())
		mAnimations[mActiveIndex].render(rect, flip);
}

void Animator::render(RectF rect, SDL_RendererFlip flip, Uint8 alpha) const
{
	if (isStarted())
		mAnimations[mActiveIndex].render(rect, flip, alpha);
}

void Animator::render(RectF rect, SDL_RendererFlip flip, RenderColour colourMod) const
{
	if (isStarted())
		mAnimations[mActiveIndex].render(rect, flip, colourMod);
}

void Animator::render(RectF rect, double rotation, VectorF aboutPoint) const
{
	if (isStarted())
		mAnimations[mActiveIndex].render(rect, rotation, aboutPoint);
}


void Animator::selectAnimation(Animation::Action state)
{
	for (int i = 0; i < mAnimations.size(); i++)
	{
		if (mAnimations[i].mState == state)
		{
			mActiveIndex = i;
			mAnimations[i].mLoops = 0;
			return;
		}
	}

	DebugPrint(Warning, "Animation state %d not present in animator\n", (int)state);
}


const Animation* Animator::getAnimation(Animation::Action action) const
{
	for (int i = 0; i < mAnimations.size(); i++)
	{
		if (mAnimations[i].mState == action)
		{
			return &mAnimations[i];
		}
	}

	DebugPrint(Warning, "No animation with action %d\n", action);
	return nullptr;	
}

float Animator::animationTime(Animation::Action action) const
{
	float time = -1.0f;
	const Animation* animation = getAnimation(action);

	if (animation)
		time = animation->mFrameCount * mFrameTime;
	else
		DebugPrint(Warning, "No animation with action %d\n", action);

	return time;	
}


void Animator::startAnimation(Animation::Action state)
{
	selectAnimation(state);
	start();
}

void Animator::stop()
{
	mTime = 0.0f;
	mState = TimeState::Stopped;
	mAnimations[mActiveIndex].reset();
}


void Animator::slowUpdate(float dt)
{
	if (mState == TimeState::Running)
		mTime += dt;

	if (mTime >= mFrameTime / speedFactor)
	{
		mAnimations[mActiveIndex].nextFrame();
		mTime = 0.0f;
	}
}


void Animator::clear()
{
	mAnimations.clear();
	mActiveIndex = 0;
	speedFactor = 1.0f;
	mFrameTime = -1.0f;
	mTime = 0.0f;
	mState = TimeState::Stopped;
}


void Animator::reset()
{
	for (int i = 0; i < mAnimations.size(); i++)
	{
		mAnimations[i].reset();
	}

	mTime = 0.0f;
	speedFactor = 1.0f;
	mState = TimeState::Stopped;
}