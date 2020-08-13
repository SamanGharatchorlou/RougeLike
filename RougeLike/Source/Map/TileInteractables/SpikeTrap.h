#pragma once

#include "Trap.h"

class DecorMap;


class SpikeTrap : public Trap
{
public:
	SpikeTrap() : mRecoveryTime(-1.0f) { }

	void clear() override;
	void update(EffectPool* effects) override;

	void fillData(const DecorMap& data) override;

	void trigger() override;
	
	bool canReset() const override { return mTimer.getSeconds() > mRecoveryTime && exhausted; }
	void reset() override;

private:
	float mRecoveryTime;
};