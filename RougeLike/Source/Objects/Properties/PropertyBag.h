#pragma once


#include "Objects/Bags/DataBag.h"
#include "Property.h"

using PropertyMap = std::unordered_map<PropertyType, float>;


class ValueBag : public DataBag
{
public:
	ValueBag() { };
	ValueBag(XMLNode node);
	ValueBag(ValueMap valueMap) { mData = valueMap; };
	~ValueBag() { };

	float get(const BasicString& value) const;

	bool contains(const BasicString& value) const override { return mData.count(value) > 0; }
	bool isEmpty()							const override { return mData.size() == 0; }


protected:
	void fillData(const StringMap& stringMap) override;

private:
	ValueMap mData;
};


class PropertyBag : public DataBag
{
public:
	PropertyBag() { };
	PropertyBag(XMLNode node);
	PropertyBag(const ValueMap& valueMap);
	~PropertyBag() { };

	float get(const BasicString& value) const;
	float get(PropertyType property) const;

	bool contains(const BasicString& value) const override { return mData.count(value) > 0; }
	bool contains(PropertyType property) const { return mData.count(property) > 0; }

	bool isEmpty()					const override { return mData.size() == 0; }



protected:
	void fillData(const StringMap& stringMap) override;

private:
	PropertyMap mData;
};



