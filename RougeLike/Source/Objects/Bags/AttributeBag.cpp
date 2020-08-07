#include "pch.h"
#include "AttributeBag.h"

#include "Objects/Properties/Attributes/Attribute.h"
#include "Objects/Properties/Attributes/Armor.h"
#include "Objects/Properties/Attributes/Health.h"
#include "Objects/Properties/Attributes/Level.h"
#include "Objects/Properties/Attributes/Damage.h"


AttributeBag::~AttributeBag()
{
	for (AttributeMap::iterator iter = mData.begin(); iter != mData.end(); iter++)
	{
		Attribute* attribute = iter->second;
		delete attribute;
	}
}


Attribute* AttributeBag::get(AttributeType attribute) const
{
	AttributeMap::const_iterator iter = mData.find(attribute);
	if (iter != mData.end())
	{
		return iter->second;
	}
	else
	{
		DebugPrint(Warning, "The attribute map does not contain the '%s' property, returning nullptr\n", attribute.string().c_str());
		return nullptr;
	}
}

float AttributeBag::value(AttributeType attribute) const
{
	Attribute* attributeObj = get(attribute);
	return attributeObj ? attributeObj->value() : NULL;
}


void AttributeBag::fillData(const StringMap& stringMap)
{
	for (StringMap::const_iterator iter = stringMap.begin(); iter != stringMap.end(); iter++)
	{
		BasicString name = iter->first;
		float value = atof(iter->second.c_str());

		Attribute* attribute = getNewAttribute(name);
		attribute->init(value);
		mData[name] = attribute;
	}
}


Attribute* AttributeBag::getNewAttribute(AttributeType attributeType) const
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
	case AttributeType::Level:
		attribute = new Level;
		break;
	default:
		attribute = new AttributeValue;
		break;
	}

	return attribute;
}
