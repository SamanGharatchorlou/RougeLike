#pragma once

#include "Utilities/Maps/DecorMap.h"
#include "Collisions/EffectCollider.h"

class Map;
class MapTile;
class EffectPool;

enum class TrapType
{
	None,
	Spike,

};

class Trap
{
public:
	Trap(MapTile* tile);
	virtual ~Trap() { }

	virtual void fillData(const DecorMap& data) = 0;
	virtual void init(EffectPool* effects) = 0;

	virtual void update(EffectPool* pool) { } // TODO: hacked this in??

	void set();
	virtual void trigger() = 0;
	virtual void reset() = 0;

	virtual bool canTrigger() const { return mTimer.getSeconds() > mTriggerTime && !exhausted; }
	virtual bool canReset() const { return mTimer.getSeconds() > mRecoveryTime && exhausted; }

	virtual bool didHit() { return mCollider.didHit(); }
	virtual bool gotHit() { return mCollider.gotHit(); }

	EffectCollider* collider() { return &mCollider; }

	const MapTile* tile() const { return mTile; }


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
	SpikeTrap(MapTile* tile) : Trap(tile) { }

	void fillData(const DecorMap& data) override;
	void init(EffectPool* effects) override;

	void trigger() override;
	void reset() override;

private:
	PropertyMap info;
};


// TODO link the floor burst and damage effect to this!
class FireGratingTrap : public Trap
{
public:
	FireGratingTrap(MapTile* tile) : Trap(tile), mActive(false), mLoops(0) { }

	void fillData(const DecorMap& data) override;
	void init(EffectPool* effects) override;

	void update(EffectPool* pool);

	void trigger() override;
	void reset() override;

	bool canTrigger() const override { return mActive && !exhausted; }
	bool canReset() const override { return false; }
	
	void setActive(bool isActive) { mActive = isActive; }

private:
	PropertyMap info;
	bool mActive;

	int mLoops;
};


class FireTrapTrigger : public Trap
{
public:
	FireTrapTrigger(MapTile* tile, std::vector<Trap*>& traps) : Trap(tile), mTraps(traps) { }

	void fillData(const DecorMap& data) override;
	void init(EffectPool* effects) override { }

	void trigger() override;
	void reset() override;

	bool canReset() const override { return false; }

private:
	std::vector<Trap*>& mTraps;
};