#pragma once

#include "Objects/Bags/DataBag.h"
#include "Objects/Properties/Property.h"


using PropertyMap = std::unordered_map<PropertyType, float>;


class PropertyBag : public DataBag
{
public:
	PropertyBag() { };
	PropertyBag(XMLNode node);
	PropertyBag(const FloatMap& valueMap);
	~PropertyBag() { };

	float get(PropertyType property) const;

	bool contains(PropertyType property) const { return mData.count(property) > 0; }

	bool isEmpty()					const override { return mData.size() == 0; }



protected:
	void fillData(const StringMap& stringMap) override;

private:
	PropertyMap mData;
};



