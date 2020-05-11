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


/*

	std::string name = "";
	float value = 0.0f;

	for (ValueMap::iterator iter = valueMap.begin(); iter != valueMap.end(); iter++)
	{
		name = iter->first;
		value = iter->second;
		PropertyType type = mPropertyTypeTable[name];

		switch (type)
		{
		case PropertyType::Level:
		{
			Level* level = new Level();
			mProperties.push_back(level);
			break;
		}
		case PropertyType::AttackDamage:
		case PropertyType::Score:
		case PropertyType::Experience:
		case PropertyType::TackleChargeTime:
		case PropertyType::TackleDistance:
		case PropertyType::TackleSpeed:
		case PropertyType::KnockbackDistance:
		case PropertyType::SightRange:
		case PropertyType::ChaseRange:
		case PropertyType::IdleTime:
		case PropertyType::HurtTime:
		case PropertyType::AttentionTime
		default:
			break;
		}
	}

	PropertyBag::fillProperties(valueMap);*/