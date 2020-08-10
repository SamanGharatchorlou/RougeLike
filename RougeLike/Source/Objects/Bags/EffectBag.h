#pragma once
#include "PropertyBag.h"

#include "Objects/Effects/EffectTypes/EffectTypes.h"
#include "Objects/Effects/EffectPool.h"


class EffectBag
{
public:
	void readEffects(XMLNode parerntEffectNode);
	void empty();

	const std::unordered_map<EffectType, PropertyBag>& effects() { return mEffects; }

	const PropertyBag& effect(EffectType type) { return mEffects[type]; }
	int size() { mEffects.size(); }

private:
	std::unordered_map<EffectType, PropertyBag> mEffects;

};
