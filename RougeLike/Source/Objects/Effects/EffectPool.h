#pragma once

#include "Effect.h"

class EffectPool
{
public:
	EffectPool();

	//void 


private:
	std::unordered_map<EffectType, std::vector<Effect*>> mPool;
};