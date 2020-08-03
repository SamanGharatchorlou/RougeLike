#pragma once

#include "Objects/Properties/Property.h"

using PropertyMap = std::unordered_map<BasicString, Property*>;


class DataBag
{
public:
	void readData(const XMLParser& parser, const BasicString& nodeName);
	virtual ~DataBag() { };

	virtual bool isEmpty() const = 0;

protected:
	virtual StringMap readValues(xmlNode node) const;
	virtual void fillData(const StringMap& stringMap) = 0;
};


class ValueBag : public DataBag
{
public:
	~ValueBag() { };

	float get(const BasicString& value) const;
	bool isEmpty() const override { return mData.size() == 0; }

protected:
	void fillData(const StringMap& stringMap) override;

private:
	ValueMap mData;
};


// TODO: use polymorphism below
class PropertyBag : public DataBag
{
public:
	virtual ~PropertyBag();

	Property* get(const BasicString& name) const;
	float value(const BasicString& name) const;

	bool contains(const BasicString& name) const;
	bool isEmpty() const override { return mData.size() == 0; }


protected:
	void fillData(const StringMap& stringMap) override;
	Property* getNewProperty(const BasicString& name) const;

protected:
	PropertyMap mData;
};
