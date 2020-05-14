#include "pch.h"
#include "EnemyPropertyBag.h"

#include "Objects/Attributes/Damage.h"


void EnemyPropertyBag::readProperties(const std::string& config)
{
	mConfigFile = config;

	ValueMap map = readConfigValues(FileManager::Config_Enemies, config);

	fillProperties(map);
}


void EnemyPropertyBag::fillProperties(ValueMap& valueMap)
{
	for (ValueMap::iterator iter = valueMap.begin(); iter != valueMap.end(); iter++)
	{
		std::string name = iter->first;
		Value value = iter->second;

		if (name == "Damage")
		{
			Damage* damage = new Damage(value.first);
			mProperties[name] = damage;

			// Set has been visited bool to true
			iter->second.second = true;
		}
	}

	PropertyBag::fillProperties(valueMap);
}
