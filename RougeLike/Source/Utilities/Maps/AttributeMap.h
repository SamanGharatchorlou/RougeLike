#pragma once

#include "DataMap.h"
#include "Objects/Properties/Property.h"

#include "Objects/Properties/Attributes/Attribute.h"



class AttributeMap : public DataMap<AttributeType, Attribute*>
{
public:
	virtual ~AttributeMap();

	void fill(const XMLNode& node) override;

	float value(AttributeType attribute) const { return at(attribute)->value(); }


protected:
	Attribute* getNewAttribute(AttributeType attribute) const;
};
