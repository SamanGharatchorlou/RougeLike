#pragma once

#include "DataBag.h"
#include "Objects/Properties/Property.h"

class Attribute;

using AttributeMap = std::unordered_map<AttributeType, Attribute*>;


class AttributeBag : public DataBag
{
public:
	virtual ~AttributeBag();

	Attribute* get(AttributeType attribute) const;
	float value(AttributeType attribute) const;

	bool contains(AttributeType attribute) const { return mData.count(attribute) > 0; }
	bool isEmpty()						   const override { return mData.size() == 0; }


protected:
	void fillData(const StringMap& stringMap) override;
	Attribute* getNewAttribute(AttributeType attribute) const;


protected:
	AttributeMap mData;
};
