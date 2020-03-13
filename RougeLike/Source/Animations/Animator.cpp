#include "pch.h"
#include "Animator.h"


void Animator::init(Tileset spriteSheet, Animations animations)
{
	mSpriteSheet = spriteSheet;
	mAnimations = animations;
}

void Animator::slowUpdate(float dt)
{
	if (timer.getSeconds() >= mActiveAnimation.speed / speedFactor)
	{
		if (++animationIndex >= mActiveAnimation.startingIndex + mActiveAnimation.count)
		{
			animationIndex = mActiveAnimation.startingIndex;
		}

		timer.restart();
	}
}


void Animator::selectAnimation(std::string name)
{
	timer.restart();
	mActiveAnimation = mAnimations[name];
	animationIndex = mActiveAnimation.startingIndex;
}


Tile* Animator::getSpriteTile()
{
	return &mSpriteSheet.getTile(animationIndex);
}


Texture* Animator::getSpriteTexture()
{
	return mSpriteSheet.getTile(animationIndex).getTexture();
}


void Animator::reset()
{
	animationIndex = 0;
	timer.restart();
}

