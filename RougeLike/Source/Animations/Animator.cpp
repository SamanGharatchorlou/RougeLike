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
		if (++animationIndex >= animation.startingIndex + animation.count)
		{
			animationIndex = animation.startingIndex;
		}

		timer.restart();
	}
}

void Animator::clear()
{
	mActiveAnimation = "";
	animationIndex = 0;
	speedFactor = 1.0f;
	timer.stop();
}


void Animator::selectAnimation(const std::string& name)
{
	if (mActiveAnimation != name)
	{
		timer.restart();
		mActiveAnimation = name;
		animationIndex = mAnimations[mActiveAnimation].startingIndex;
	}
}


Tile* Animator::getSpriteTile()
{
	return &mSpriteSheet.getTile(animationIndex);
}


Texture* Animator::getSpriteTexture()
{
	return mSpriteSheet.getTile(animationIndex).getTexture();
}


