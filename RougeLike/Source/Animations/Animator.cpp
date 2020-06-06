#include "pch.h"
#include "Animator.h"

// temp
#include "Graphics/Texture.h"




Animator2::Animation::Animation(AnimationData& data) : mTexture(data.texture), mTileDimentions(data.tileDimentions), mState(data.state), mCount(0)
{


}

void Animator2::Animation::render(RectF rect)
{
#if _DEBUG
	Vector2D<int> requestSize = (mIndex + 1)*mTileDimentions;
	Vector2D<int> objectSize = mTexture->originalDimentions;

	if (requestSize.x > objectSize.x || requestSize.y > objectSize.y)
		DebugPrint(Error, "Index(%d,%d) out of bounds\n", mIndex.x, mIndex.y);
#endif

	VectorF size = mTileDimentions; // keep as ints?
	VectorF position = mTileDimentions * mIndex;
	RectF tileRect(position, size);

	mTexture->renderSubTexture(rect, tileRect);
}


void Animator2::Animation::nextFrame()
{
	Index bounaries = mTexture->originalDimentions / mTileDimentions;


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
	}
}





Animator2::Animator2() : mActiveIndex(0), speedFactor(0.0f)
{

}

void Animator2::addAnimation(AnimationData& data)
{
	Animation animation(data);
	mAnimations.push_back(animation);
}




void Animator2::selectAnimation(AnimationState state)
{
	for (int i = 0; i < mAnimations.size(); i++)
	{
		if (mAnimations[i].state() == state)
		{
			mActiveIndex = i;
			return;
		}
	}

	DebugPrint(Warning, "Animation state %d not present in animator\n", (int)state);
}





// --- Animator --- //


void Animator::init(Tileset spriteSheet, Animations animations)
{
	mSpriteSheet = spriteSheet;
	mAnimations = animations;
}

void Animator::slowUpdate(float dt)
{
	AnimationInfo& animation = mAnimations[mActiveAnimation];

	if (timer.getSeconds() >= animation.speed / speedFactor)
	{
		if (++mAnimationIndex >= animation.startingIndex + animation.count)
		{
			mAnimationIndex = animation.startingIndex;
		}

		timer.restart();
	}
}

void Animator::clear()
{
	mActiveAnimation = "";
	mAnimationIndex = 0;
	speedFactor = 1.0f;
	timer.stop();
}


void Animator::selectAnimation(const std::string& name)
{
	if (mActiveAnimation != name)
	{
		if (mAnimations.count(name) > 0)
		{
			timer.restart();
			mActiveAnimation = name;
			mAnimationIndex = mAnimations[mActiveAnimation].startingIndex;
		}
		else
			DebugPrint(Warning, "No animation named '%s' available in animation map\n", name.c_str());
	}
}


Tile* Animator::getSpriteTile()
{
	return &mSpriteSheet.getTile(mAnimationIndex);
}


Texture* Animator::getSpriteTexture()
{
	return mSpriteSheet.getTile(mAnimationIndex).getTexture();
}


void Animator::render(RectF rect)
{
	mSpriteSheet.getTile(mAnimationIndex).render(rect);
}