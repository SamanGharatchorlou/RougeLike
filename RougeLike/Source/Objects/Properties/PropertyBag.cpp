#include "pch.h"
#include "PropertyBag.h"

#include "Objects/Attributes/Level.h"
#include "Objects/Attributes/Health.h"
#include "Objects/Attributes/Damage.h"
#include "Objects/Attributes/Armor.h"


// ---------- DataBag --------------

void DataBag::readData(const XMLParser& parser, const BasicString& nodeName)
{
	XMLNode propertyNode = parser.root().first(nodeName);
	if (propertyNode)
	{
		StringMap map = readValues(propertyNode);
		fillData(map);
	}
	else
	{
		DebugPrint(Warning, "XML file has no '%s' node name\n", nodeName.c_str());
	}
}


// Pull raw config data from file into code
StringMap DataBag::readValues(XMLNode node) const
{
	StringMap stringMap;
	XMLNode valueNode = node.first();

	while (valueNode)
	{
		stringMap[valueNode.name()] = valueNode.value();
		valueNode = valueNode.next();
	}

	return stringMap;
}


// ---------- ValueBag --------------
ValueBag::ValueBag(XMLNode node)
{
	StringMap data = readValues(node);
	fillData(data);
}


void ValueBag::fillData(const StringMap& stringMap)
{
	for (StringMap::const_iterator iter = stringMap.begin(); iter != stringMap.end(); iter++)
	{
		BasicString name = iter->first;
		float value = atof(iter->second.c_str());
		mData[name] = value;
	}
}

float ValueBag::get(const BasicString& value) const
{
	if (mData.count(value))
	{
		return mData.at(value);
	}
	else
	{
		DebugPrint(Warning, "Value bag does not contain value with label '%s'\n", value.c_str());
		return -1.0f;
	}
}


// ---------- PropertyBag --------------

PropertyBag::~PropertyBag()
{
	for (PropertyMap::iterator iter = mData.begin(); iter != mData.end(); iter++)
	{
		Property* property = iter->second;
		delete property;
	}
}


Property* PropertyBag::get(const BasicString& name) const
{
	PropertyMap::const_iterator iter = mData.find(name);

	if (iter != mData.end())
	{
		return iter->second;
	}
	else
	{
		DebugPrint(Warning, "The property map does not contain the '%s' property, returning nullptr\n", name.c_str());
		return nullptr;
	}
}

float PropertyBag::value(const BasicString& name) const
{
	Property* property = get(name);
	return property ? property->value() : NULL;
}


bool PropertyBag::contains(const BasicString& name) const
{
	return mData.count(name) > 0 ? true : false;
}


void PropertyBag::fillData(const StringMap& stringMap)
{
	for (StringMap::const_iterator iter = stringMap.begin(); iter != stringMap.end(); iter++)
	{
		BasicString name = iter->first;
		float value = atof(iter->second.c_str());

		Property* property = getNewProperty(name);
		property->init(value);
		mData[name] = property;
	}
}


Property* PropertyBag::getNewProperty(const BasicString& name) const
{
	Property* property = nullptr;

	if (name == "Health")
	{
		property = new Health;
	}
	else if (name == "Damage")
	{
		property = new Damage;
	}
	else if (name == "Armor")
	{
		property = new Armor;
	}
	else if (name == "Level")
	{
		property = new Level;
	}
	else
	{
		property = new PropertyValue;
	}

	return property;
}
