#include "pch.h"
#include "PlayerPropertyBag.h"

#include "Objects/Attributes/Level.h"


void PlayerPropertyBag::readProperties(const std::string& config)
{
	mConfigFile = config;

	ValueMap map = readConfigValues(FileManager::Config_Player, config);

	fillProperties(map);
}


void PlayerPropertyBag::fillProperties(ValueMap& valueMap)
{
	for (ValueMap::iterator iter = valueMap.begin(); iter != valueMap.end(); iter++)
	{
		std::string name = iter->first;
		Value value = iter->second;

		if (name == "Level")
		{
			Level* level = new Level;
			mProperties[name] = level;

			// Set has been visited bool to true
			iter->second.second = true;
		}
	}

	PropertyBag::fillProperties(valueMap);
}