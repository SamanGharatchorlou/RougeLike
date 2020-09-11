#include "pch.h"
#include "Animation.h"

#include "Graphics/Texture.h"


Animation::Animation(const Data& data) :
	mTexture(data.texture), mTileDimentions(data.tileDimentions),
	mState(data.action), mFrameCount(data.frameCount),
	mLoops(0) { }


void Animation::reset()
{
	mIndex = Index(0, 0);
	mLoops = 0;
}

void Animation::render(RectF rect, SDL_RendererFlip flip) const
{
	RectF tileRect = subTileRect(rect);
	RectF renderRect = autoSize(rect);

	mTexture->renderSubTexture(renderRect, tileRect, flip);
}


void Animation::render(RectF rect, SDL_RendererFlip flip, Uint8 alpha) const
{
	RectF tileRect = subTileRect(rect);
	mTexture->renderSubTexture(rect, tileRect, flip, alpha);
}

void Animation::render(RectF rect, SDL_RendererFlip flip, RenderColour colourMod) const
{
	RectF tileRect = subTileRect(rect);
	mTexture->renderSubTexture(rect, tileRect, flip, colourMod);
}

void Animation::render(RectF rect, double rotation, VectorF aboutPoint) const
{
	RectF tileRect = subTileRect(rect);
	mTexture->renderSubTexture(rect, tileRect, rotation, aboutPoint);
}


void Animation::nextFrame()
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


int Animation::currentFrame() const
{
	Index bounaries = (mTexture->originalDimentions / mTileDimentions).toInt();
	return (mIndex.y * bounaries.x) + mIndex.x;
}


// -- Private Functions -- //

// Resize and reposition texture to keep fixed ratio and center within rect
RectF Animation::autoSize(RectF rect) const
{
	VectorF size = mTileDimentions;
	VectorF theSize = realiseSize(size, std::max(rect.Width(), rect.Height()));
	VectorF offset = (rect.Size() - theSize) / 2.0f;
	return RectF(rect.TopLeft() + offset, theSize);
}


RectF Animation::subTileRect(RectF rect) const
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



Animation::Action stringToAction(const BasicString& action)
{
	if (action == "Idle")
		return Animation::Idle;
	else if (action == "Walk")
		return Animation::Walk;
	else if (action == "Run")
		return Animation::Run;
	else if (action == "Attack")
		return Animation::Attack;
	else if (action == "Alert")
		return Animation::Alert;
	else if (action == "Hurt")
		return Animation::Hurt;
	else if (action == "Dead")
		return Animation::Dead;
	else if (action == "Active")
		return Animation::Active;
	else
	{
		DebugPrint(Warning, "No action matched the string '%s'\n", action.c_str());
		return Animation::None;
	}
}