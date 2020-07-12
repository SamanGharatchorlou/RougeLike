#pragma once

#include "Objects/Properties/Property.h"

using PropertyMap = std::unordered_map<std::string, Property*>;


class PropertyBag
{
public:
	using ValueMap = std::unordered_map<std::string, float>;


public:
	PropertyBag() : mConfigFile("") { }
	virtual ~PropertyBag() { }

	virtual void readProperties(const std::string& config);
	
	void resetProperties();

	Property* get(const std::string& name) const;
	float value(const std::string& name) const;

	bool contains(const std::string& name) const;


protected:
	virtual ValueMap readValues(XMLParser& parser);
	
	virtual void fillProperties(ValueMap& valueMap);

	Property* getNewProperty(const std::string& name);

protected:
	std::string mConfigFile;
	PropertyMap mProperties;
};


//class EffectPropertyBag : public PropertyBag
//{
//public:
//	EffectPropertyBag() { }
//
//	void readProperties(const std::string& config) override { };
//
//	void setProperty(const std::string& name, float value);
//
//
//private:
//	ValueMap readValues(XMLParser& parser) override;
//};