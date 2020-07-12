#pragma once

#include "EffectTypes/Effect.h"

struct GameData;


class EffectPool
{
public:
	EffectPool(GameData* gameData);

	void slowUpdate();

	Effect* getEffect(EffectType type);

	void returnEffect(Effect* effect);

private:
	Effect* getNewEffect(EffectType type);


private:
	GameData* mGameData;

	// Effects are pulled from this pool then returned when finished with
	std::unordered_map<EffectType, std::queue<Effect*>> mPool;
	std::unordered_map<EffectType, int> mPoolSizes;

#if _DEBUG // Tracker contains all effects at all times
	std::unordered_map<EffectType, std::queue<Effect*>> mTrackerPool;
#endif
};