#include "pch.h"
#include "PropertyBag.h"

#include "Objects/Attributes/Health.h"
#include "Objects/Attributes/Level.h"


PropertyBag::PropertyBag()
{
	mPropertyTypeTable["Level"] = PropertyType::Level;
	mPropertyTypeTable["Health"] = PropertyType::Health;
	mPropertyTypeTable["AttackSpeed"] = PropertyType::AttackSpeed;
	mPropertyTypeTable["Force"] = PropertyType::Force;
	mPropertyTypeTable["MaxVelocity"] = PropertyType::MaxVelocity;
}


PropertyBase* PropertyBag::get(PropertyType type) const
{
	for (int i = 0; i < mProperties.size(); i++)
	{
		if (mProperties[i]->type() == type)
			return mProperties[i];
	}

	DebugPrint(Log, "No property of type %d containted within the property list\n", type);
}


void PropertyBag::fillProperties(ValueMap& valueMap)
{
	std::string name = "";
	float value = 0.0f;

	for (ValueMap::iterator iter = valueMap.begin(); iter != valueMap.end(); iter++)
	{
		name = iter->first;
		value = iter->second;
		PropertyType type = mPropertyTypeTable[name];

		switch (type)
		{
		case PropertyType::Health:
		{
			Health* health = new Health(value);
			mProperties.push_back(health);
			break;

		}
		case PropertyType::Level:
		{
			Level* level = new Level();
			mProperties.push_back(level);
			break;
		}
		case PropertyType::AttackDamage:
		case PropertyType::AttackSpeed:
		case PropertyType::Force:
		case PropertyType::MaxVelocity:
		{
			PropertyValue* property = new PropertyValue(type);
			property->setValue(value);
			mProperties.push_back(property);
			break;
		}
		default:
			break;
		}

	}

}