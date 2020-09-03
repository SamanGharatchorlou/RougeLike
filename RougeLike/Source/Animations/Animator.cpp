#include "pch.h"
#include "Animator.h"

#include "Graphics/Texture.h"


Action stringToAction(const BasicString& action)
{
	if (action == "Idle")
		return Action::Idle;	
	else if (action == "Walk")
		return Action::Walk;
	else if (action == "Run")
		return Action::Run;
	else if (action == "Attack")
		return Action::Attack;
	else if (action == "Alert")
		return Action::Alert;
	else if (action == "Hurt")
		return Action::Hurt;
	else if (action == "Dead")
		return Action::Dead;
	else if (action == "Active")
		return Action::Active;
	else
	{
		DebugPrint(Warning, "No action matched the string '%s'\n", action.c_str());
		return Action::None;
	}
}


// --- Animation --- //
Animator::Animation::Animation(const AnimationData& data) : 
	mTexture(data.texture), mTileDimentions(data.tileDimentions), 
	mState(data.action), mFrameCount(data.frameCount),
	mLoops(0) { }


void Animator::Animation::reset()
{
	mIndex = Index(0, 0);
	mLoops = 0;
}

void Animator::Animation::render(RectF rect, SDL_RendererFlip flip) const
{
	RectF tileRect = subTileRect(rect);
	RectF renderRect = autoSize(rect);

	mTexture->renderSubTexture(renderRect, tileRect, flip);
}


void Animator::Animation::render(RectF rect, SDL_RendererFlip flip, Uint8 alpha) const
{
	RectF tileRect = subTileRect(rect);
	// TODO: does this auto size work correctly and should I always use it?
	//RectF renderRect = autoSize(rect);

	mTexture->renderSubTexture(rect, tileRect, flip, alpha);
}

void Animator::Animation::render(RectF rect, SDL_RendererFlip flip, RenderColour colourMod) const
{
	RectF tileRect = subTileRect(rect);
	// TODO: does this auto size work correctly and should I always use it?
	//RectF renderRect = autoSize(rect);

	mTexture->renderSubTexture(rect, tileRect, flip, colourMod);
}


// NOTE: is the sub texture size correct??? the height seems off?
void Animator::Animation::render(RectF rect, double rotation, VectorF aboutPoint) const
{
	RectF tileRect = subTileRect(rect);
	RectF renderRect = autoSize(rect);

	mTexture->renderSubTexture(renderRect, tileRect, rotation, aboutPoint);
}


// Resize and reposition texture to keep fixed ratio and center within rect
RectF Animator::Animation::autoSize(RectF rect) const
{
	VectorF size = mTileDimentions;
	VectorF theSize = realiseSize(size, std::max(rect.Width(), rect.Height()));
	VectorF offset = (rect.Size() - theSize) / 2.0f;
	return RectF(rect.TopLeft() + offset, theSize);
}


RectF Animator::Animation::subTileRect(RectF rect) const
{
#if DEBUG_CHECK
	VectorF requestSize = (mIndex + 1).toFloat() * mTileDimentions;
	VectorF objectSize = mTexture->originalDimentions;

	if (requestSize.x > objectSize.x || requestSize.y > objectSize.y)
		DebugPrint(Error, "Index(%d,%d) out of bounds\n", mIndex.x, mIndex.y);
#endif

	VectorF size = mTileDimentions;
	VectorF position = mTileDimentions * mIndex;
	return RectF(position, size);
}


void Animator::Animation::nextFrame()
{
	Index bounaries = (mTexture->originalDimentions / mTileDimentions).toInt();
	mIndex += Index(1, 0);

	Index yIncrement(0, 0);
	if (mIndex.x >= bounaries.x)
	{
		mIndex.x = 0;
		yIncrement += Index(0, 1);
	}

	mIndex += yIncrement;

	if (mIndex.y >= bounaries.y)
	{
		mIndex.y = 0;
		mIndex.x = 0;
		mLoops++;
	}
}


int Animator::Animation::currentFrame() const
{
	Index bounaries = (mTexture->originalDimentions / mTileDimentions).toInt();
	return (mIndex.y * bounaries.x) + mIndex.x;
}


// --- Animator --- //
Animator::Animator() : mActiveIndex(0), speedFactor(1.0f), mFrameTime(0.0f) { }


void Animator::addAnimation(const AnimationData& data)
{
	Animation animation(data);
	mAnimations.push_back(animation);
}

void Animator::render(RectF rect, SDL_RendererFlip flip) const
{
	if(timer.isStarted())
		mAnimations[mActiveIndex].render(rect, flip);
}

void Animator::render(RectF rect, SDL_RendererFlip flip, Uint8 alpha) const
{
	if (timer.isStarted())
		mAnimations[mActiveIndex].render(rect, flip, alpha);
}

void Animator::render(RectF rect, SDL_RendererFlip flip, RenderColour colourMod) const
{
	if (timer.isStarted())
		mAnimations[mActiveIndex].render(rect, flip, colourMod);
}

void Animator::render(RectF rect, double rotation, VectorF aboutPoint) const
{
	if (timer.isStarted())
		mAnimations[mActiveIndex].render(rect, rotation, aboutPoint);
}


void Animator::selectAnimation(Action state)
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


//Animator::Animation* Animator::getAnimation(Action action)
//{
//	for (int i = 0; i < mAnimations.size(); i++)
//	{
//		if (mAnimations[i].mState == action)
//		{
//			return &mAnimations[i];
//		}
//	}
//
//	DebugPrint(Warning, "No animation with action %d\n", action);
//	return nullptr;	
//}


void Animator::startAnimation(Action state)
{
	selectAnimation(state);
	start();
}

void Animator::stop()
{
	timer.stop();
	mAnimations[mActiveIndex].reset();
}


void Animator::slowUpdate(float dt)
{
	if (timer.getSeconds() >= mFrameTime / speedFactor)
	{
		mAnimations[mActiveIndex].nextFrame();
		timer.restart();
	}
}


void Animator::clear()
{
	mAnimations.clear();
	mActiveIndex = 0;
	speedFactor = 1.0f;
	mFrameTime = -1.0f;
	timer.stop();
}


void Animator::reset()
{
	for (int i = 0; i < mAnimations.size(); i++)
	{
		mAnimations[i].reset();
	}

	speedFactor = 1.0f;
	timer.stop();
}