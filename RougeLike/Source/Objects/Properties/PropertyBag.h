#pragma once

#include "Objects/Attributes/Property.h"
#include "Objects/Properties/PropertyBase.h"

using ValueMap = std::unordered_map<std::string, float>;


class PropertyBag
{
public:
	PropertyBag();

	PropertyBase* get(PropertyType type) const;

	std::vector<PropertyBase*> mProperties;

	std::unordered_map<std::string, PropertyType> mPropertyTypeTable;

	void fillProperties(ValueMap& valueMap);
};