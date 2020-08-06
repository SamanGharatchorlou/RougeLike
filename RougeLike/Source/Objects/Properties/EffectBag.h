#pragma once
#include "PropertyBag.h"

#include "Objects/Effects/EffectTypes/EffectTypes.h"
#include "Objects/Effects/EffectPool.h"


class EffectBag
{
public:
	void readEffects(XMLNode parerntEffectNode);

	const std::unordered_map<EffectType, ValueBag>& effects() { return mEffects; }

	const ValueBag& effect(EffectType type) { return mEffects[type]; }
	int size() { mEffects.size(); }

private:
	std::unordered_map<EffectType, ValueBag> mEffects;

};
