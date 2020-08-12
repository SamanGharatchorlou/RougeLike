#include "pch.h"
#include "EffectMap.h"

void EffectMap::fill(const XMLNode& node)
{
	if (node)
	{
		XMLNode effectNode = node.child();
		while (effectNode)
		{
			PropertyMap effectValues(effectNode);

			BasicString effect = effectNode.name();
			EffectType type = EffectType::None;
			type << effect;

			mData[type] = effectValues;

			effectNode = effectNode.next();
		}
	}
}
