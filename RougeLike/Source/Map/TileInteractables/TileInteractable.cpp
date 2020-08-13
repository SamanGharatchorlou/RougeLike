#include "pch.h"
#include "TileInteractable.h"
#include "Map/Tiles/MapTile.h"


void TileInteractable::init(MapTile* tile)
{
	mTile = tile;
	mCollider.init(&tile->rectRef());
}

void TileInteractable::clear()
{
	mTile = nullptr;
	mType = DecorType::None;
	exhausted = false;
	mCollider = EffectCollider();
}