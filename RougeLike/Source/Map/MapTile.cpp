#include "pch.h"
#include "MapTile.h"
#include "Graphics/Texture.h"


// --- PathTile --- //
void PathTile::add(CollisionTile type)
{
	mCollisionType = mCollisionType | type;
}

bool PathTile::has(CollisionTile type) const
{
	return (mCollisionType & type) != CollisionTile::None;
}

//bool PathTile::is(CollisionTile type) const
//{
//	return mRenderType == type;
//}

void PathTile::remove(CollisionTile type)
{
	mCollisionType = mCollisionType & ~type;
}


// --- MapTile --- //
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



void MapTile::render(RectF rect) { mTexture->render(rect); }