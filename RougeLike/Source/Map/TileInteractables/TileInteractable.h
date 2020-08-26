#pragma once

#include "Collisions/Colliders/EffectCollider.h"
#include "Map/Tiles/TileTypes.h"

class MapTile;


enum class TileInteractionType
{
	None,
	Trap,
	Trigger
};


class TileInteractable
{
public:
	TileInteractable() : mType(DecorType::None), mTile(nullptr), exhausted(false) { }
	virtual ~TileInteractable() { }

	virtual void clear();

	void setType(DecorType type) { mType = type; }
	void init(MapTile* tile);


	EffectCollider* collider() { return &mCollider; }

	virtual bool canTrigger() const = 0;
	virtual void trigger() = 0;

	virtual bool didHit() const { return mCollider.didHit(); }
	virtual bool gotHit() const { return mCollider.gotHit(); }

	const MapTile* tile() const { return mTile; }

	DecorType type() const { return mType; }

protected:
	MapTile* mTile;
	DecorType mType;

	bool exhausted;
	EffectCollider mCollider;

};