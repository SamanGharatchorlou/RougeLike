#include "pch.h"
#include "EffectBag.h"

void EffectBag::readEffects(XMLNode parentEffectNode)
{
	if (parentEffectNode)
	{
		XMLNode effectNode = parentEffectNode.child();
		while (effectNode)
		{
			PropertyBag effectValues(effectNode);

			BasicString effect = effectNode.name();
			EffectType type = EffectType::None;
			type << effect;

			mEffects[type] = effectValues;

			effectNode = effectNode.next();
		}
	}
}

void EffectBag::empty()
{
	mEffects.clear();
}