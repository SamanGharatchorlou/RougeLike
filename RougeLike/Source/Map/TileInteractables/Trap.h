#pragma once

#include "TileInteractable.h"

#include "Utilities/Maps/DecorMap.h"

class Map;
class EffectPool;


class Trap : public TileInteractable
{
public:
	Trap() : mTriggerTime(-1.0f) { }
	virtual ~Trap() { }

	virtual void clear() override
	{
		mTimer.stop();
		mTriggerTime = -1.0f;
		info.empty();
		TileInteractable::clear();
	}

	virtual void fillData(const DecorMap& data) = 0;

	virtual void update(EffectPool* pool) = 0;

	void set()
	{
		mTimer.start();
		mCollider.reset();
	}

	virtual void reset() = 0;

	virtual bool canTrigger() const { return mTimer.getSeconds() > mTriggerTime && !exhausted; }
	virtual bool canReset() const { return false; }


protected:
	PropertyMap info;
	TimerF mTimer;
	float mTriggerTime;
};

