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


// --- AnimationTile --- //
bool MapTile::is(AnimationTile type) const
{
	return mAnimationType == type;
}


void MapTile::render(RectF rect) 
{
#if _DEBUG
	if(mTexture)
#endif
		mTexture->render(rect); 
}