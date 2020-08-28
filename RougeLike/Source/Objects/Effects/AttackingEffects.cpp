#include "pch.h"
#include "AttackingEffects.h"
#include "Objects/Effects/EffectTypes/EffectTypes.h"


void AttackingEffects::fill(const XMLNode& node)
{
	XMLNode typeNode = node.child("Types");
	XMLNode type = typeNode.child();

	while (type)
	{
		EffectType effectType = EffectType::None;
		effectType << type.name();

		mTypes.push_back(effectType);

		type = type.next();
	}

	XMLNode infoNode = node.child("Info");
	mData.fill(infoNode);
}


void AttackingEffects::clear()
{
	mTypes.clear();
	mData.empty();
}