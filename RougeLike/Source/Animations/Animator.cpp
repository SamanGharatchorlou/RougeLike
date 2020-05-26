#include "pch.h"
#include "Animator.h"


void Animator::init(Tileset spriteSheet, Animations animations)
{
	mSpriteSheet = spriteSheet;
	mAnimations = animations;
}

void Animator::slowUpdate(float dt)
{
	Animation& animation = mAnimations[mActiveAnimation];

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