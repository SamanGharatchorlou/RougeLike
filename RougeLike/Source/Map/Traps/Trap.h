#pragma once

#include "Utilities/Maps/DecorMap.h"
#include "Collisions/EffectCollider.h"

class Map;
class MapTile;
class EffectPool;

class Trap
{
public:
	Trap(MapTile* tile);
	virtual ~Trap() { }

	virtual void fillData(const DecorMap& data) = 0;
	virtual void init(EffectPool* effects) = 0;

	void set();
	virtual void trigger() = 0;
	virtual void reset() = 0;

	bool canTrigger() const { return mTimer.getSeconds() > mTriggerTime && !exhausted; }
	bool canReset() const { return mTimer.getSeconds() > mRecoveryTime && exhausted; }

	bool didHit() { return mCollider.didHit(); }
	bool gotHit() { return mCollider.gotHit(); }

	EffectCollider* collider() { return &mCollider; }


protected:
	MapTile* mTile;

	TimerF mTimer;
	float mTriggerTime;
	float mRecoveryTime;

	bool exhausted;

	EffectCollider mCollider;
};


class SpikeTrap : public Trap
{
public:
	SpikeTrap(MapTile* tile) : Trap(tile){ }

	void fillData(const DecorMap& data) override;
	void init(EffectPool* effects) override;

	void trigger() override;
	void reset() override;

private:
	PropertyMap info;
};


// TODO link the floor burst and damage effect to this!
//class FireGratingTrap : public Trap
//{
//public:
//	FireGratingTrap(MapTile* tile) : Trap(tile) { }
//
//	void fillData(const DecorMap& data) override;
//	void init(EffectPool* effects) override;
//
//	void trigger() override;
//	void reset() override;
//
//private:
//	PropertyMap info;
//};


class SpikeTrapTrigger : public Trap
{
public:
	SpikeTrapTrigger(MapTile* tile, Map* map) : Trap(tile), mMap(map) { }

	void fillData(const DecorMap& data) override;
	void init(EffectPool* effects) override { }

	void trigger() override;
	void reset() override;

private:
	Map* mMap;
};