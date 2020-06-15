#include "pch.h"
#include "EffectPool.h"

#include "DamageEffect.h"

EffectPool::EffectPool()
{
	int poolSize = 3;

	for (EffectType i = EffectType::None; i < EffectType::Count; i = i + 1)
	{
		if (i == EffectType::Damage)
		{
			std::vector<Effect*> effects;

			for (int i = 0; i < poolSize; i++)
			{
				effects.push_back(new DamageEffect);
			}

			mPool[i] = effects;
		}
	}
}

