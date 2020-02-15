#include "pch.h"
#include "Animator.h"


void Animator::init(Tileset spriteSheet, Animations animations)
{
	mSpriteSheet = spriteSheet;
	mAnimations = animations;
}

void Animator::slowUpdate(float dt)
{
	if (timer.getSeconds() >= mActiveAnimation.speed)
	{
		if (++animationIndex >= mActiveAnimation.startingIndex + mActiveAnimation.count)
		{
			animationIndex = mActiveAnimation.startingIndex;
			loops++;
		}

		timer.restart();
	}
}


void Animator::selectAnimation(std::string name)
{
	timer.restart();
	loops = 0;
	mActiveAnimation = mAnimations[name];
	animationIndex = mActiveAnimation.startingIndex;
}


const Tile* Animator::getSpriteTile()
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
	loops = 0;
	timer.restart();
}

