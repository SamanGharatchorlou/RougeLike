#include "pch.h"
#include "EffectBag.h"


void EffectBag::readEffects(XMLNode parentEffectNode)
{
	if (parentEffectNode)
	{
		XMLNode effectNode = parentEffectNode.first();
		while (effectNode)
		{
			ValueBag effectValues(effectNode);

			BasicString effect = effectNode.name();
			EffectType type = EffectType::None;
			type << effect;

			mEffects[type] = effectValues;

			effectNode = effectNode.next();
		}
	}
}