#pragma once

#include "DataMap.h"
#include "Objects/Properties/Property.h"


class PropertyMap : public DataMap<PropertyType, float>
{
public:
	PropertyMap() { };
	PropertyMap(XMLNode node);
	//PropertyMap(const FloatMap& valueMap);

	void fill(const XMLNode& node) override;
};



