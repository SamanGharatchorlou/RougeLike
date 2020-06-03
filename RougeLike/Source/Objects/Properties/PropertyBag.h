#pragma once

#include "Objects/Properties/Property.h"

using PropertyMap = std::unordered_map<std::string, Property*>;


class PropertyBag
{
public:
	using Value = std::pair<float, bool>;
	using ValueMap = std::unordered_map<std::string, Value>;


public:
	virtual ~PropertyBag() { }

	virtual void readProperties(const std::string& config) = 0;	
	
	void resetProperties();

	Property* get(const std::string& name) const;
	float value(const std::string& name) const;

	bool contains(const std::string& name) const;


protected:
	ValueMap readConfigValues(FileManager::Folder folder, const std::string& config);
	void fillProperties(ValueMap& valueMap);


protected:
	std::string mConfigFile;
	PropertyMap mProperties;
};