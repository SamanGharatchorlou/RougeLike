#pragma once

#include "EffectTypes/Effect.h"


class EffectPool
{
public:
	EffectPool();

	void load();
	void slowUpdate();

	Effect* getEffect(EffectType type);
	void returnEffect(Effect* effect);

private:
	Effect* getNewEffect(EffectType type);


private:
	// Effects are pulled from this pool then returned when finished with
	std::unordered_map<EffectType, std::queue<Effect*>> mPool;
	std::unordered_map<EffectType, int> mPoolSizes;

#if _DEBUG // Tracker contains all effects at all times
	std::unordered_map<EffectType, std::queue<Effect*>> mTrackerPool;
#endif
};