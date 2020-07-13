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
void MapTile::add(DecorTile type)
{
	mDecorType = mDecorType | type;
}

bool MapTile::has(DecorTile type) const
{
	return (mDecorType & type) != DecorTile::None;
}

bool MapTile::is(DecorTile type) const
{
	return mDecorType == type;
}



// --- MapTile --- //
void MapTile::addAnimation(Animator animation)
{
	animation.start();
	mAnimations.push_back(animation);
}

void MapTile::slowUpdate(float dt)
{
	for (int i = 0; i < mAnimations.size(); i++)
	{
		mAnimations[i].slowUpdate(dt);
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
		if(is(DecorTile::Torch_Bowl))
			rect.SetTopCenter(rect.Center());

		mAnimations[i].render(rect);
	}
}