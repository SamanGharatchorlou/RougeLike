#include "pch.h"
#include "PropertyBag.h"

#include "Objects/Attributes/Health.h"


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


/// --- Private Functions --- ///
PropertyBag::ValueMap PropertyBag::readValues(XMLParser& parser)
{
	ValueMap valueMap;
	float value = 0.0f;

	xmlNode propertyNode = parser.rootNode()->first_node("Properties");
	xmlNode node = propertyNode->first_node();

	while (node != nullptr)
	{
		std::string name = node->name();
		float nodeValue = std::stof(node->value());
		bool hasBeenRead = false;

		valueMap[name] = Value(nodeValue, hasBeenRead);

		node = node->next_sibling();
	}

	return valueMap;
}


void PropertyBag::fillProperties(ValueMap& valueMap)
{
	for (ValueMap::iterator iter = valueMap.begin(); iter != valueMap.end(); iter++)
	{
		Value value = iter->second;

		if (value.second == false)
		{
			std::string name = iter->first;

			if (name == "Health")
			{
				Health* health = new Health(value.first);
				mProperties[name] = health;
			}
			else if (name == "Damage")
			{
				Damage* damage = new Damage(value.first);
				mProperties[name] = damage;
			}
			else
			{
				PropertyValue* property = new PropertyValue(value.first);
				mProperties[name] = property;
			}
		}
	}
}