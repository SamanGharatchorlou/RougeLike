#include "pch.h"
#include "PropertyBag.h"


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
PropertyBag::PropertyBag(XMLNode node)
{
	StringMap data = readValues(node);
	fillData(data);
}



PropertyBag::PropertyBag(const ValueMap& valueMap)
{
	ValueMap::const_iterator iter;
	for (iter = valueMap.begin(); iter != valueMap.end(); iter++)
	{
		mData[iter->first] = iter->second;
	}
}


float PropertyBag::get(const BasicString& value) const
{
	if (mData.count(value))
	{
		return mData.at(value);
	}
	else
	{
		DebugPrint(Warning, "Property bag does not contain value with label '%s'\n", value.c_str());
		return -1.0f;
	}
}

float PropertyBag::get(PropertyType property) const
{
	if (mData.count(property))
	{
		return mData.at(property);
	}
	else
	{
		DebugPrint(Warning, "Value bag does not contain value with label '%s'\n", property.string().c_str());
		return -1.0f;
	}
}

void PropertyBag::fillData(const StringMap& stringMap)
{
	for (StringMap::const_iterator iter = stringMap.begin(); iter != stringMap.end(); iter++)
	{
		const BasicString name(iter->first);
		float value = atof(iter->second.c_str());
		mData[name] = value;
	}
}

