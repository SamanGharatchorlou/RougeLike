#include "pch.h"
#include "MapTile.h"


// --- PathTile --- //
void PathTile::addCollisionType(Type type)
{
	mCollisionType |= type;
}

bool PathTile::hasCollisionType(Type type) const
{
	return (mCollisionType & type) != MapTile::None;
}

void PathTile::removeCollisionType(Type type)
{
	mCollisionType &= ~type;
}


// --- MapTile --- //
bool MapTile::hasRenderType(Type type) const 
{
	return (mRenderType & type) != MapTile::None;
}

bool MapTile::isRenderType(Type type) const
{
	return mRenderType == type;
}

void MapTile::addRenderType(Type type)
{
	mRenderType |= type;
}

void MapTile::removeRenderType(Type type)
{
	mRenderType &= ~type;
}

