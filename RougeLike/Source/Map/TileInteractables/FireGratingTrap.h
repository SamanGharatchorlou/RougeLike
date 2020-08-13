#pragma once

#include "Trap.h"

class FireGratingTrap : public Trap
{
public:
	FireGratingTrap() : mActive(false), mLoops(0) { }

	void clear() override;
	void fillData(const DecorMap& data) override;

	void update(EffectPool* pool);

	void trigger() override;
	void reset() override { }

	bool canTrigger() const override { return mActive && !exhausted; }
	bool canReset() const override { return false; }

	void setActive(bool isActive) { mActive = isActive; }

private:
	bool mActive;
	int mLoops;
};