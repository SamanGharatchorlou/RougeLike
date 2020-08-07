#pragma once

#include "Objects/Properties/Property.h"
#include "Objects/Properties/PropertyTypes/PropertyEnums.h"


using PropertyMap = std::unordered_map<BasicString, Property*>;
//using PropertyMap = std::unordered_map<PropertyType, Property*>;


class DataBag
{
public:
	virtual void readData(const XMLParser& parser, const BasicString& nodeName);
	virtual ~DataBag() { };

	virtual bool contains(const BasicString& name) const = 0;
	virtual bool isEmpty() const = 0;

protected:
	virtual StringMap readValues(XMLNode node) const;

	virtual void fillData(const StringMap& stringMap) = 0;
};


class ValueBag : public DataBag
{
public:
	ValueBag() { };
	ValueBag(XMLNode node);
	ValueBag(ValueMap valueBag) { mData = valueBag; };
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
	virtual ~PropertyBag();

	Property* get(const BasicString& name) const;
	float value(const BasicString& name) const;

	bool contains(const BasicString& name) const override { return mData.count(name) > 0; }
	bool isEmpty()						   const override { return mData.size() == 0; }


protected:
	void fillData(const StringMap& stringMap) override;
	Property* getNewProperty(const BasicString& name) const;


protected:
	PropertyMap mData;
};


