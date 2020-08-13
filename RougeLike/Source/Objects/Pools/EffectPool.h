#pragma once

#include "Utilities/ObjectPool.h"

class Effect;
enum class EffectType;


class EffectPool : public ObjectPool<Effect, EffectType>
{
private:
	Effect* createNewObject(EffectType type) const override;
};

