#include "pch.h";
#include "Property.h"


PropertyType::PropertyType(const BasicString& name) : type(getType(name)) { }


PropertyType::Type PropertyType::getType(const BasicString& name) const
{
	return findPropertyType(name);
}

BasicString PropertyType::string() const
{
	return findPropertyString(type);
}


PropertyType::Type findPropertyType(const BasicString& string)
{
#if _DEBUG
	if (PropertyLookUpTable.count(string) == 0)
	{
		DebugPrint(Log, "No property in the lookup table with string '%s'\n", string.c_str());
		return PropertyType::None;
	}
	else
#endif
		return PropertyLookUpTable.at(string);
}


BasicString findPropertyString(PropertyType::Type type)
{
	std::unordered_map<BasicString, PropertyType::Type>::const_iterator iter;
	for (iter = PropertyLookUpTable.begin(); iter != PropertyLookUpTable.end(); iter++)
	{
		if (type == iter->second)
			return iter->first;
	}

	DebugPrint(Log, "No property in the lookup table with type '%d'\n", type);
	return "";
}



AttributeType::AttributeType(PropertyType::Type propertyType) : PropertyType(propertyType) { }
AttributeType::AttributeType(const BasicString& name) : PropertyType(name) { }


bool operator == (PropertyType propertyA, PropertyType propertyB)
{
	return propertyA.type == propertyB.type;
}

bool operator == (AttributeType attributeA, AttributeType attributeB)
{
	return attributeA.type == attributeB.type;
}