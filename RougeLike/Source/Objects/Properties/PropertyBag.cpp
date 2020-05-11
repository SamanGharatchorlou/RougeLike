#include "pch.h"
#include "PropertyBag.h"

#include "Objects/Attributes/Health.h"



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
	readProperties(mConfigFile);
}



/// --- Private Functions --- ///
ValueMap PropertyBag::readConfigValues(FileManager::Folder folder, const std::string& config)
{
	std::string configFilePath = FileManager::Get()->filePath(folder, config);

	XMLParser parser;
	parser.parseXML(configFilePath);

	ValueMap valueMap;
	float value = 0.0f;

	xmlNode rootNode = parser.rootNode();
	xmlNode node = rootNode->first_node();

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
			else
			{
				PropertyValue* property = new PropertyValue(value.first);
				mProperties[name] = property;
			}
		}
	}
}