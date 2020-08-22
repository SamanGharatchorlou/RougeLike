#pragma once

#include "DataMap.h"
#include "Objects/Properties/Property.h"


class PropertyMap : public DataMap<PropertyType, float>
{
public:
	PropertyMap() { };
	PropertyMap(XMLNode node);

	void addXYPosition(VectorF position);

	void fill(const XMLNode& node) override;
};



