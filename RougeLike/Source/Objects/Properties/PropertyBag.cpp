#include "pch.h"
#include "PropertyBag.h"

#include "Objects/Attributes/Level.h"
#include "Objects/Attributes/Health.h"
#include "Objects/Attributes/Damage.h"
#include "Objects/Attributes/Armor.h"


void PropertyBag::readProperties(const std::string& config)
{
	mConfigFile = config;
	XMLParser parser(FileManager::Get()->findFile(FileManager::Configs_Objects, config));

	ValueMap map = readValues(parser);
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
PropertyBag::ValueMap PropertyBag::readValues(XMLParser& parser)
{
	xmlNode propertyNode = parser.rootNode()->first_node("Properties");
	xmlNode node = propertyNode->first_node();

	ValueMap valueMap;

	while (node != nullptr)
	{
		std::string name = node->name();
		float nodeValue = std::stof(node->value());

		valueMap[name] = nodeValue;

		node = node->next_sibling();
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



// Effect stuff
/*
PropertyBag::ValueMap EffectPropertyBag::readValues(XMLParser& parser)
{
	xmlNode propertyNode = parser.rootNode()->first_node("Effects");
	xmlNode node = propertyNode->first_node();

	ValueMap valueMap;

	while (node != nullptr)
	{
		std::string name = node->name();
		float nodeValue = std::stof(node->value());

		valueMap[name] = nodeValue;

		node = node->next_sibling();
	}

	return valueMap;
}



void EffectPropertyBag::setProperty(const std::string& name, float value)
{
	if (contains(name))
	{
		Property* property = get(name);
		//TODO: should this be init?
		property->setValue(value);
	}
	else
	{
		Property* property = getNewProperty(name);
		property->init(value);
		mProperties[name] = property;
	}
}
*/
