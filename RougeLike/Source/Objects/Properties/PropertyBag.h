#pragma once

#include "Objects/Properties/Property.h"

using PropertyMap = std::unordered_map<std::string, Property*>;


class XMLDataBag
{
public:
	void readData(const XMLParser& parser, const std::string& nodeName);


protected:
	virtual StringMap readValues(xmlNode node) const;
	virtual void fillData(const StringMap& stringMap) = 0;
};


class ValueBag : public XMLDataBag
{
public:
	float get(const std::string& value) const;

protected:
	void fillData(const StringMap& stringMap) override;

private:
	ValueMap mData;
};


class PropertyBag
{
public:
	PropertyBag() : mConfigFile("") { }
	virtual ~PropertyBag() { }

	virtual void readProperties(XMLParser& parser);
	virtual void readProperties(const std::string& config);
	
	void resetProperties();

	Property* get(const std::string& name) const;
	float value(const std::string& name) const;

	bool contains(const std::string& name) const;


protected:
	virtual ValueMap readValues(xmlNode node);
	virtual void fillProperties(ValueMap& valueMap);

	Property* getNewProperty(const std::string& name);

protected:
	std::string mConfigFile;
	PropertyMap mProperties;
};
