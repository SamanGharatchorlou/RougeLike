#pragma once

#include "Utilities/ObjectPool.h"
#include "EffectTypes/EffectTypes.h"

class Effect;


class EffectPool : public ObjectPool<Effect, EffectType>
{
public:
	EffectPool() { }

	void load() override;

	void returnObject(Effect* effect) override;

private:
	Effect* createNewObject(EffectType type) const override;
};

