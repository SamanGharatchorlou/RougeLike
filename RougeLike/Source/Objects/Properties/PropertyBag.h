#pragma once

#include "Objects/Attributes/Property.h"

using Value = std::pair<float, bool>;
using ValueMap = std::unordered_map<std::string, Value>;
using PropertyMap = std::unordered_map<std::string, Property*>;


class PropertyBag
{
public:
	virtual void readProperties(const std::string& config) = 0;	
	
	void resetProperties();

	Property* get(const std::string& name) const;
	float value(const std::string& name) const;

protected:
	ValueMap readConfigValues(FileManager::Folder folder, const std::string& config);
	void fillProperties(ValueMap& valueMap);

protected:
	std::string mConfigFile;
	PropertyMap mProperties;
};