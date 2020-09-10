#include "pch.h"
#include "AttributeMap.h"

#include "StringMap.h"

#include "Objects/Properties/Attributes/Attribute.h"
#include "Objects/Properties/Attributes/Armor.h"
#include "Objects/Properties/Attributes/Health.h"
#include "Objects/Properties/Attributes/Damage.h"


AttributeMap::~AttributeMap()
{
	clear();
}


void AttributeMap::clear()
{
	for (iterator iter = mData.begin(); iter != mData.end(); iter++)
	{
		Attribute* attribute = iter->second;
		delete attribute;
	}

	empty();
}


void AttributeMap::fill(const XMLNode& node)
{
	StringMap map(node);
	for (StringMap::const_iterator iter = map.begin(); iter != map.end(); iter++)
	{
		BasicString name = iter->first;
		float value = (float)atof(iter->second.c_str());

		Attribute* attribute = getNewAttribute(name);
		attribute->init(value);
		mData[name] = attribute;
	}
}


Attribute* AttributeMap::getNewAttribute(AttributeType attributeType) const
{
	Attribute* attribute = nullptr;

	switch (attributeType.type)
	{
	case AttributeType::Health:
		attribute = new Health;
		break;
	case AttributeType::Damage:
		attribute = new Damage;
		break;
	case AttributeType::Armor:
		attribute = new Armor;
		break;
	default:
		attribute = new AttributeValue;
		break;
	}

	return attribute;
}
