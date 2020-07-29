#include "pch.h"
#include "PropertyBag.h"

#include "Objects/Attributes/Level.h"
#include "Objects/Attributes/Health.h"
#include "Objects/Attributes/Damage.h"
#include "Objects/Attributes/Armor.h"


// ------------------------

void XMLDataBag::readData(const XMLParser& parser, const std::string& nodeName)
{
	xmlNode propertyNode = parser.rootNode()->first_node(nodeName.c_str());
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
StringMap XMLDataBag::readValues(xmlNode node) const
{
	StringMap stringMap;
	xmlNode valueNode = node->first_node();

	while (valueNode != nullptr)
	{
		std::string name = valueNode->name();
		std::string value = valueNode->value();
		stringMap[name] = value;

		valueNode = valueNode->next_sibling();
	}

	return stringMap;
}


// ------------------------


void ValueBag::fillData(const StringMap& stringMap)
{
	for (StringMap::const_iterator iter = stringMap.begin(); iter != stringMap.end(); iter++)
	{
		std::string name = iter->first;
		float value = std::stof(iter->second);
		mData[name] = value;
	}
}


float ValueBag::get(const std::string& value) const
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


// ------------------------


void PropertyBag::readProperties(const std::string& config)
{
	mConfigFile = config;
	XMLParser parser(FileManager::Get()->findFile(FileManager::Configs_Objects, config));

	xmlNode propertyNode = parser.rootNode()->first_node("Properties");
	ValueMap map = readValues(propertyNode);
	fillProperties(map);
}

void PropertyBag::readProperties(const XMLParser& parser)
{
	xmlNode propertyNode = parser.rootNode()->first_node("Properties");
	ValueMap map = readValues(propertyNode);
	fillProperties(map);
}


Property* PropertyBag::get(const std::string& name) const
{
	PropertyMap::const_iterator iter = mProperties.find(name);

	if (iter != mProperties.end())
	{
		return iter->second;
	}
	else
	{
		DebugPrint(Warning, "The property map does not contain the '%s' property, returning nullptr\n", name.c_str());
		return nullptr;
	}
}

float PropertyBag::value(const std::string& name) const
{
	Property* property = get(name);
	return property ? property->value() : NULL;
}


void PropertyBag::resetProperties()
{
	mProperties.clear();
	readProperties(mConfigFile);
}


bool PropertyBag::contains(const std::string& name) const
{
	return mProperties.count(name) > 0 ? true : false;
}


// --- Private Functions --- //


// Pull raw config data from file into code
ValueMap PropertyBag::readValues(xmlNode node)
{
	ValueMap valueMap;
	xmlNode valueNode = node->first_node();

	while (valueNode != nullptr)
	{
		std::string name = valueNode->name();
		float nodeValue = std::stof(valueNode->value());

		valueMap[name] = nodeValue;

		valueNode = valueNode->next_sibling();
	}

	return valueMap;
}


void PropertyBag::fillProperties(ValueMap& valueMap)
{
	for (ValueMap::iterator iter = valueMap.begin(); iter != valueMap.end(); iter++)
	{
		std::string name = iter->first;
		float value = iter->second;

		Property* property = getNewProperty(name);
		property->init(value);
		mProperties[name] = property;
	}
}


Property* PropertyBag::getNewProperty(const std::string& name)
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
