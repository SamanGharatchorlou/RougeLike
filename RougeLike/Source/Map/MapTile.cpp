#include "pch.h"
#include "MapTile.h"

bool MapTile::hasRenderType(Type type) const 
{
	return (mRenderType & type) != MapTile::None;
}

void MapTile::addRenderType(Type type)
{
	mRenderType |= type;
}

void MapTile::removeRenderType(Type type)
{
	mRenderType &= ~type;
}

void MapTile::addCollisionType(Type type)
{
	mCollisionType |= type;
}

bool MapTile::hasCollisionType(Type type) const
{
	return (mCollisionType & type) != MapTile::None;
}

void MapTile::removeCollisionType(Type type)
{
	mCollisionType &= ~type;
}