#include "pch.h"
#include "MapTile.h"
#include "Graphics/Texture.h"


// --- CollisionTile --- //
void PathTile::add(CollisionTile type)
{
	mCollisionType = mCollisionType | type;
}

bool PathTile::is(CollisionTile type) const
{
	return mCollisionType == type;
}

bool PathTile::has(CollisionTile type) const
{
	return (mCollisionType & type) != CollisionTile::None;
}

void PathTile::remove(CollisionTile type)
{
	mCollisionType = mCollisionType & ~type;
}


// --- RenderTile --- //
bool MapTile::has(RenderTile type) const
{
	return (mRenderType & type) != RenderTile::None;
}

bool MapTile::is(RenderTile type) const
{
	return mRenderType == type;
}

void MapTile::add(RenderTile type)
{
	mRenderType = mRenderType | type;
}

void MapTile::remove(RenderTile type)
{
	mRenderType = mRenderType & ~type;
}


// --- DecorationTile --- //
void MapTile::add(DecorType type)
{
	mDecorType = mDecorType | type;
}

bool MapTile::has(DecorType type) const
{
	return (mDecorType & type) != DecorType::None;
}

bool MapTile::is(DecorType type) const
{
	return mDecorType == type;
}



// --- MapTile --- //
void MapTile::addAnimation(Animator animation)
{
	//animation.start();
	mAnimations.push_back(animation);
}

void MapTile::slowUpdate(float dt)
{
	for (int i = 0; i < mAnimations.size(); i++)
	{
		mAnimations[i].slowUpdate(dt);

		if (has(DecorType::Grating))
		{
			mDeferredRendering = true;
		}
	}
}

void MapTile::render(RectF rect) 
{
#if _DEBUG
	if(mTexture)
#endif
		mTexture->render(rect); 

	for (int i = 0; i < mAnimations.size(); i++)
	{
		if (has(DecorType::GratingTrigger))
		{
			int a = 4;
		}

		if (mDeferredRendering)
			return;

		if(has(DecorType::Torch))
			rect.SetBotCenter(rect.Center());

		mAnimations[i].render(rect);
	}
}


void MapTile::deferredRender(RectF rect)
{
	for (int i = 0; i < mAnimations.size(); i++)
	{
		if (has(DecorType::Grating))
		{
			rect.SetBotCenter(rect.Center());
		}

		mAnimations[i].render(rect);
	}
}