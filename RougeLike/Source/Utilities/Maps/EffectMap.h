#pragma once

#include "PropertyMap.h"
#include "Objects/Effects/EffectTypes/EffectTypes.h"


class EffectMap : public DataMap<EffectType, PropertyMap>
{
public:
	void fill(const XMLNode& node) override;

};
