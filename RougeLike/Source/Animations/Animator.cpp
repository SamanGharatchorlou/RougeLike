#include "pch.h"
#include "Animator.h"

// temp
#include "Graphics/Texture.h"

Action stringToAction(const std::string& action)
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
Animator::Animation::Animation(AnimationData& data) : 
	mTexture(data.texture), mTileDimentions(data.tileDimentions), 
	mState(data.action), mFrameCount(data.frameCount),
	mLoops(0) {
	printf("animation created\n");
}


void Animator::Animation::render(RectF rect, SDL_RendererFlip flip) const
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

	mTexture->renderSubTexture(rect, tileRect, flip);
}


void Animator::Animation::render(RectF rect, SDL_RendererFlip flip, Uint8 alpha)
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

	mTexture->renderSubTexture(rect, tileRect, flip, alpha);
}


void Animator::Animation::nextFrame()
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
		mLoops++;
	}
}


int Animator::Animation::currentFrame() const
{
	Index bounaries = mTexture->originalDimentions / mTileDimentions;
	return (mIndex.y * bounaries.x) + mIndex.x + 1;
}


// --- Animator --- //
Animator::Animator() : mActiveIndex(0), speedFactor(1.0f), mFrameSpeed(0.0f) { }


void Animator::addAnimation(AnimationData& data)
{
	Animation animation(data);
	mAnimations.push_back(animation);
}

void Animator::render(RectF rect, SDL_RendererFlip flip) const
{
	mAnimations[mActiveIndex].render(rect, flip);
}

void Animator::render(RectF rect, SDL_RendererFlip flip, Uint8 alpha)
{
	mAnimations[mActiveIndex].render(rect, flip, alpha);
}


void Animator::selectAnimation(Action state)
{
	for (int i = 0; i < mAnimations.size(); i++)
	{
		if (mAnimations[i].mState == state)
		{
			mActiveIndex = i;
			return;
		}
	}

	DebugPrint(Warning, "Animation state %d not present in animator\n", (int)state);
}

void Animator::startAnimation(Action state)
{
	for (int i = 0; i < mAnimations.size(); i++)
	{
		if (mAnimations[i].mState == state)
		{
			mActiveIndex = i;
			mAnimations[i].mLoops = 0;
			start();
			return;
		}
	}

	DebugPrint(Warning, "Animation state %d not present in animator\n", (int)state);
}


void Animator::slowUpdate(float dt)
{
	float frameSpeed = mFrameSpeed * 20 / mAnimations[mActiveIndex].mFrameCount;

	if (timer.getSeconds() >= mFrameSpeed / speedFactor)
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
	mFrameSpeed = -1.0f;
	timer.stop();
	mRect = nullptr;
}


// --- Animator --- //

/*

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
*/